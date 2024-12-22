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

void Server::read_client(int fd)
{
    ssize_t n = read(fd, message_buffer, sizeof(message_buffer));
    auto ptr_handler = handlers_[acceptor_.get_socket_fd()];
    

    if(n > 0)
    {
        message_buffer[n] = 0;
        
        ptr_handler->setWriteCallback(std::bind(&Server::write_client, this, std::placeholders::_1));
        ptr_handler->enable_write();
        reactor_.register_handler(ptr_handler.get());
    }
    else if(n == 0){
        printf("close fd\n");
        ::close(acceptor_.get_socket_fd());
        reactor_.remove_handler(ptr_handler.get());
        handlers_.erase(acceptor_.get_socket_fd());
    }
    else{
        ::close(fd);
        printf("read error\n");
        reactor_.remove_handler(ptr_handler.get());
        handlers_.erase(fd);
    }
}

void Server::write_client(int fd)
{
    int nbytes = write(fd, message_buffer, 3);
    // int nbytes = 1;
    auto ptr_handler = handlers_[fd];
    if(nbytes > 0)
    {
        ptr_handler->setReadCallback(std::bind(&Server::read_client, this, std::placeholders::_1));
        ptr_handler->enable_read();
        reactor_.register_handler(ptr_handler.get());
    }
    else {
        printf("write error\n");
        ::close(fd);
        reactor_.register_handler(ptr_handler.get());
        handlers_.erase(fd);
    }
}
