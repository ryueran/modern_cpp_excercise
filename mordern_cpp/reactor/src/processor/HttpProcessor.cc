#include "processor/HttpProcessor.hh"
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

void HttpProcessor::process_message(
    ReuqestType request_type,
    const std::string& message,
    int status,
    int fd,
    const std::string& file_path
)
{
    if(request_type == ReuqestType::VISITREQUEST)
    {
        send_response(fd, message, status);
    } else if (request_type == ReuqestType::DOWNLOADREQUEST)
    {
        handle_file_download(fd, file_path);
    } else {
        // do nothing
    }
}

void HttpProcessor::send_response(int fd, const std::string& message, int status)
{
    std::string status_text = (status == 200) ? "OK" :
                              (status == 404) ? "Not Found" :
                              (status == 405) ? "Method Not Allowed" : "Internal Server Error";

    std::ostringstream response;
    response << "HTTP/1.1 " << status << " " << status_text << "\r\n"
             << "Content-Type: text/plain\r\n"
             << "Content-Length: " << message.size() << "\r\n"
             << "Connection: close\r\n"
             << "\r\n"
             << message;

    std::string response_str = response.str();
    write(fd, response_str.c_str(), response_str.size());
}

void HttpProcessor::handle_file_download(int fd, const std::string& file_path)
{
    std::string absolute_file_path = root_path_ + file_path;
    std::cout << "root path: " << root_path_ << std::endl;
    std::cout << "Resolved file path: " << absolute_file_path << std::endl;

    std::ifstream file(absolute_file_path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        send_response(fd, "404 Not Found", 404);
        return;
    }

    // get file size
    std::streamsize file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    // read file content
    std::vector<char> buffer(file_size);
    if (!file.read(buffer.data(), file_size)) {
        send_response(fd, "500 Internal Server Error", 500);
        return;
    }

    // create http response
    std::ostringstream response;
    response << "HTTP/1.1 200 OK\r\n"
             << "Content-Type: application/octet-stream\r\n"
             << "Content-Length: " << file_size << "\r\n"
             << "Content-Disposition: attachment; filename=\"" << absolute_file_path.substr(absolute_file_path.find_last_of('/') + 1) << "\"\r\n"
             << "Connection: close\r\n"
             << "\r\n";

    // send response header
    std::string header = response.str();
    write(fd, header.c_str(), header.size());

    // send response content
    write(fd, buffer.data(), file_size);
}