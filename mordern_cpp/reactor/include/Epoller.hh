#ifndef EPOLLER_HH
#define EPOLLER_HH

#include <iostream>
#include <memory>
#include <vector>
#include <sys/epoll.h>

#define MAX_EVENTS 20

class Handler;
class Epoller
{
public:
    Epoller(int fd): epfd_(fd) {
        std::cout << "Epoller constructed!" << std::endl;
    }

    ~Epoller()
    {
        std::cout << "Epoller destructed!" << std::endl;
    }

    void update(std::shared_ptr<Handler>& ptr_handler);
    void remove(std::shared_ptr<Handler>& ptr_handler);
    std::vector<std::shared_ptr<Handler>> poll();

private:
    int epfd_;
    std::vector<epoll_event> epoll_events_; 
};

#endif