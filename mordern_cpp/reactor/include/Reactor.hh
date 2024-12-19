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
        epoll_fd_ = epoll_create1(0);
        ptr_epoller.reset(new Epoller(epoll_fd_));
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