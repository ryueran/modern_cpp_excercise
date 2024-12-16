#include "Epoller.hh"
#include "Handler.hh"

std::vector<std::shared_ptr<Handler>> Epoller::poll()
{
    std::vector<std::shared_ptr<Handler>> handler_list = {};
    int n_event = epoll_wait(epfd_, epoll_events_.data(), MAX_EVENTS, -1); // epoll_event consumer
    if (n_event == -1) {
        perror("epoll_wait");
        return handler_list;
    }

    for(auto event : epoll_events_)
    {
        std::shared_ptr<Handler> handler_ptr(static_cast<Handler*>(event.data.ptr));
        if (event.events & EPOLLIN)
        {
            handler_ptr->enable_read();
        }
        if(event.events & EPOLLOUT)
        {
            handler_ptr->enable_write();
        }
        handler_list.push_back(handler_ptr);
    }
    return handler_list;
}
