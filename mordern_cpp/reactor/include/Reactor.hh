#ifndef REACTOR_HH
#define REACTOR_HH

#include <iostream>
#include <vector>
#include <memory>
#include <unistd.h>
#include "Handler.hh"
#include "Epoller.hh"

class Reactor
{
public:
    Reactor()
    {
        std::cout << "Reactor constructed!" << std::endl;
        epoll_fd_ = epoll_create1(EPOLL_CLOEXEC);
        std::cout << "Epoll fd transferred!" << std::endl;
        ptr_epoller = std::unique_ptr<Epoller>(new Epoller(epoll_fd_));
    }

    ~Reactor()
    {
        std::cout << "Reactor destructed!" << std::endl;
        close(epoll_fd_);
    }

    void register_handler(Handler* ptr_handler);
    void remove_handler(Handler* ptr_handler);
    void loop();
private:
    std::unique_ptr<Epoller> ptr_epoller;
    int epoll_fd_;
};

#endif