#ifndef SERVER_HH
#define SERVER_HH

#include <iostream>
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
    void read_client(int fd); // handler->register(std::bind(write_client, this, _1))
    void write_client(int fd);
private:
    Acceptor acceptor_;
    Reactor reactor_;
    char message_buffer[1024];
    int fd_;
};

#endif