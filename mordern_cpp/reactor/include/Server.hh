#ifndef SERVER_HH
#define SERVER_HH

#include <iostream>
#include <map>
#include "Acceptor.hh"
#include "Handler.hh"
#include "Reactor.hh"

class Server
{
public:
    Server()
    {
        std::cout << "Server constructed!" << std::endl;
    }

    ~Server()
    {
        std::cout << "Server destructed!" << std::endl;
        // delete[] message_buffer;
    }
    void start_server(); // Acceptor.bind, listen, accept, handler->register(std::bind(read_client, this, _1))
    void accept(int server_fd);
    void read_client(int fd); // handler->register(std::bind(write_client, this, _1))
    void write_client(int fd);
private:
    void close_client(int fd) {::close(fd);}
    void handle_file_download(int fd, const std::string& file_path);
    void send_response(int fd, const std::string& message, int status_code);
    Acceptor acceptor_;
    Reactor reactor_;
    char message_buffer[1024];
    std::map<int, std::shared_ptr<Handler>> handlers_;
    int fd_;
    std::string root_directory;
};

#endif