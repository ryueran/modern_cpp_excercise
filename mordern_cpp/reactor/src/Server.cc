#include <functional>
#include <sstream>
#include <fstream>
#include <unistd.h>
#include <limits.h>
#include "Server.hh"

std::string get_executable_directory() {
    char path[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", path, PATH_MAX);
    if (count == -1) {
        perror("readlink error");
        return "";
    }
    std::string exe_path(path, count);
    return exe_path.substr(0, exe_path.find_last_of('/')); // 去掉文件名，保留目录部分
}

void Server::start_server()
{
    acceptor_.server_bind();
    acceptor_.server_listen();

    root_directory = get_executable_directory() + "/download";
    std::cout << "Root directory set to: " << root_directory << std::endl;
    // acceptor_.server_accept();
    // handler->register(std::bind(read_client, this, _1))
    std::shared_ptr<ReadHandler> ptr_handler = std::make_shared<ReadHandler>(acceptor_.get_server_fd());
    handlers_[acceptor_.get_server_fd()] = ptr_handler;
    ptr_handler->setCallback(std::bind(&Server::accept, this, std::placeholders::_1));
    reactor_.register_handler(ptr_handler.get());
    reactor_.loop();
}

void Server::accept(int server_fd)
{
    acceptor_.server_accept(server_fd);

    std::shared_ptr<ReadHandler> ptr_handler = std::make_shared<ReadHandler>(acceptor_.get_socket_fd());
    handlers_[acceptor_.get_socket_fd()] = ptr_handler;
    ptr_handler->setCallback(std::bind(&Server::read_client, this, std::placeholders::_1));
    reactor_.register_handler(ptr_handler.get());
}

void Server::read_client(int fd) {
    auto ptr_handler = handlers_[fd];
    ssize_t n = read(fd, ptr_handler->message_buffer, sizeof(ptr_handler->message_buffer) - 1);

    if (n > 0) {
        ptr_handler->message_buffer[n] = '\0'; // 确保字符串终止符

        // 简单解析 HTTP 请求行
        std::istringstream request_stream(ptr_handler->message_buffer);
        std::string method, url, version;
        request_stream >> method >> url >> version;

        if (method == "GET") {
            if (url == "/") {
                // 默认主页响应
                send_response(fd, "Hello World", 200);
            } else if (url.find("/download/") == 0) {
                // 处理下载请求
                std::string file_path = url.substr(9); // 假设文件在当前目录下
                handle_file_download(fd, file_path);
            } else {
                // 未知路径，返回 404
                send_response(fd, "404 Not Found", 404);
            }
        } else {
            // 不支持的方法，返回 405
            send_response(fd, "405 Method Not Allowed", 405);
        }
    } else if (n == 0) {
        // 客户端关闭连接
        reactor_.remove_handler(ptr_handler.get());
        close_client(fd);
        handlers_.erase(fd);
    } else {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return; // 等待下次事件
        }
        perror("read error");
        reactor_.remove_handler(ptr_handler.get());
        close_client(fd);
        handlers_.erase(fd);
    }
}

void Server::write_client(int fd) {
    auto ptr_handler = handlers_[fd];
    std::string response = 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: 13\r\n"
        "Connection: close\r\n"
        "\r\n"
        "Hello World";

    const char* data = response.c_str();
    size_t total_bytes = response.size();
    ssize_t bytes_written = 0;

    while (bytes_written < total_bytes) {
        ssize_t n = write(fd, data + bytes_written, total_bytes - bytes_written);
        if (n > 0) {
            bytes_written += n;
        } else if (n == -1 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
            break; // 等待下次事件
        } else {
            perror("write error");
            reactor_.remove_handler(ptr_handler.get());
            close_client(fd);
            handlers_.erase(fd);
            return;
        }
    }

    if (bytes_written == total_bytes) {
        reactor_.remove_handler(ptr_handler.get());
        close_client(fd);
        handlers_.erase(fd);
    }
}

void Server::handle_file_download(int fd, const std::string& file_path) {
    std::string absolute_file_path = root_directory + file_path;
    std::cout << "Resolved file path: " << absolute_file_path << std::endl;

    std::ifstream file(absolute_file_path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        send_response(fd, "404 Not Found", 404);
        return;
    }

    // 获取文件大小
    std::streamsize file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    // 读取文件内容
    std::vector<char> buffer(file_size);
    if (!file.read(buffer.data(), file_size)) {
        send_response(fd, "500 Internal Server Error", 500);
        return;
    }

    // 构造 HTTP 响应
    std::ostringstream response;
    response << "HTTP/1.1 200 OK\r\n"
             << "Content-Type: application/octet-stream\r\n"
             << "Content-Length: " << file_size << "\r\n"
             << "Content-Disposition: attachment; filename=\"" << absolute_file_path.substr(absolute_file_path.find_last_of('/') + 1) << "\"\r\n"
             << "Connection: close\r\n"
             << "\r\n";

    // 发送响应头
    std::string header = response.str();
    write(fd, header.c_str(), header.size());

    // 发送文件内容
    write(fd, buffer.data(), file_size);

    // 关闭连接
    reactor_.remove_handler(handlers_[fd].get());
    close_client(fd);
    handlers_.erase(fd);
}

void Server::send_response(int fd, const std::string& message, int status_code) {
    std::string status_text = (status_code == 200) ? "OK" :
                              (status_code == 404) ? "Not Found" :
                              (status_code == 405) ? "Method Not Allowed" : "Internal Server Error";

    std::ostringstream response;
    response << "HTTP/1.1 " << status_code << " " << status_text << "\r\n"
             << "Content-Type: text/plain\r\n"
             << "Content-Length: " << message.size() << "\r\n"
             << "Connection: close\r\n"
             << "\r\n"
             << message;

    std::string response_str = response.str();
    write(fd, response_str.c_str(), response_str.size());

    reactor_.remove_handler(handlers_[fd].get());
    close_client(fd);
    handlers_.erase(fd);
}
