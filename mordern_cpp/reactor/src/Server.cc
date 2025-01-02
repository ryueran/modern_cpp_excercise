#include <functional>
#include "Server.hh"

void Server::start_server()
{
    acceptor_.server_bind();
    acceptor_.server_listen();

    // acceptor_.server_accept();
    // handler->register(std::bind(read_client, this, _1))
    std::shared_ptr<Handler> ptr_handler = std::make_shared<Handler>(acceptor_.get_server_fd());
    handlers_[acceptor_.get_server_fd()] = ptr_handler;
    ptr_handler->setReadCallback(std::bind(&Server::accept, this, std::placeholders::_1));
    ptr_handler->enable_read();
    reactor_.register_handler(ptr_handler.get());
    reactor_.loop();
}

void Server::accept(int server_fd)
{
    acceptor_.server_accept(server_fd);

    std::shared_ptr<Handler> ptr_handler = std::make_shared<Handler>(acceptor_.get_socket_fd());
    handlers_[acceptor_.get_socket_fd()] = ptr_handler;
    ptr_handler->setReadCallback(std::bind(&Server::read_client, this, std::placeholders::_1));
    ptr_handler->enable_read();
    reactor_.register_handler(ptr_handler.get());
}

void Server::read_client(int fd) {
    auto ptr_handler = handlers_[fd];
    ssize_t n = read(fd, ptr_handler->message_buffer, sizeof(ptr_handler->message_buffer) - 1);

    if (n > 0) {
        ptr_handler->message_buffer[n] = '\0'; // 确保字符串终止符
        write_client(fd); // 写入响应
    } else if (n == 0) {
        // 客户端关闭连接
        close_client(fd);
        reactor_.remove_handler(ptr_handler.get());
        handlers_.erase(fd);
    } else {
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return; // 等待下次事件
        }
        perror("read error");
        close_client(fd);
        reactor_.remove_handler(ptr_handler.get());
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
            close_client(fd);
            reactor_.remove_handler(ptr_handler.get());
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
