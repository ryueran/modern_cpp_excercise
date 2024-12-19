#ifndef ACCEPTOR_HH
#define ACCEPTOR_HH

#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include "Epoller.hh"

#define PORT 8080

class Acceptor
{
public:
    Acceptor();
    ~Acceptor();
    void server_bind();
    void server_listen();
    void server_accept();
    int get_socket_fd();

private:
    sockaddr_in server_addr_;
    int server_fd_;
    int socket_fd_;
};

#endif