#ifndef ACCEPTOR_HH
#define ACCEPTOR_HH

#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>
#include "TLS/SSLConnect.h"

#define PORT 8443

class Acceptor
{
public:
    Acceptor();
    ~Acceptor();
    void server_bind();
    void server_listen();
    void server_accept(int server_fd);
    int get_socket_fd();
    int get_server_fd();
    SSL_CTX* get_context();
    SSL* get_ssl();

private:
    sockaddr_in server_addr_;
    int server_fd_;
    int socket_fd_;
    SSL_CTX* ctx;
    SSL* ssl;
};

#endif