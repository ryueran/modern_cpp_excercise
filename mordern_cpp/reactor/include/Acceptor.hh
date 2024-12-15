#ifndef ACCEPTOR_HH
#define ACCEPTOR_HH

#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>

#define MAX_EVENTS 10
#define PORT 8080

class Acceptor
{
public:
    Acceptor();
    ~Acceptor();
    void server_bind();
    void server_listen();
    void server_accept();
    // void set_handler_fd(); tech debt
private:
    sockaddr_in server_addr_;
    int server_fd_;
};

#endif