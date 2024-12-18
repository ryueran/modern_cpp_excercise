#ifndef SERVER_HH
#define SERVER_HH

#include "Acceptor.hh"
#include "Handler.hh"
#include "Reactor.hh"

class Server
{
public:
    Server();
    ~Server();
    void start_server();
    void read_client();
    void write_client();
private:
    Acceptor acceptor;
    std::unique_ptr<Reactor> ptr_reactor; 
    int fd_;
};

#endif