#include "MessageParser.hh"
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

void MessageParser::set_root_path(const std::string& root_path)
{
    root_path_ = root_path;
}

void MessageParser::parse(int fd, char *message, SSL* ssl)
{
    // parse http request
    std::istringstream request_stream(message);
    std::string method, url, version;
    request_stream >> method >> url >> version;

    if (method == "GET") {
        if (url == "/") {
            // 默认主页响应
            http_pro_->send_response(fd, "Hello World", 200, ssl);
        } else if (url.find("/download/") == 0) {
            //Safty bug technical debt
            // 处理下载请求
            std::string file_path = url.substr(9); // 假设文件在当前目录下
            std::cout << "File name is:" << file_path << std::endl;
            http_pro_->handle_file_download(fd, file_path, ssl);
        } else {
            // 未知路径，返回 404
            http_pro_->send_response(fd, "404 Not Found", 404, ssl);
        }
    } else {
        // 不支持的方法，返回 405
        http_pro_->send_response(fd, "405 Method Not Allowed", 405, ssl);
    }
}