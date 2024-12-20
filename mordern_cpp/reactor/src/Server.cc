#include <functional>
#include "Server.hh"

void Server::start_server()
{
    acceptor_.server_bind();
    acceptor_.server_listen();
    acceptor_.server_accept();
    // handler->register(std::bind(read_client, this, _1))
    Handler* ptr_handler = new Handler(acceptor_.get_socket_fd());
    ptr_handler->setReadCallback(std::bind(&Server::read_client, this, std::placeholders::_1));
    ptr_handler->enable_write();
    reactor_.register_handler(ptr_handler);
    reactor_.loop();
}

void Server::read_client(int fd)
{
    ssize_t n = read(fd, message_buffer, sizeof(message_buffer));

    Handler* ptr_handler = new Handler(fd);
    ptr_handler->setReadCallback(std::bind(&Server::write_client, this, std::placeholders::_1));
    ptr_handler->enable_write();
    reactor_.register_handler(ptr_handler);
}

void Server::write_client(int fd)
{
    write(fd, message_buffer, 1024);
}
