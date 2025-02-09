#include "Handler.hh"

void Handler::setCallback(CallBack callback)
{
    callback_ = callback;
}

int Handler::get_handler_fd()
{
    return handler_fd_;
}

void Handler::set_in_Epoll()
{
    isInEpoll_ = true;
}

void Handler::set_out_Epoll()
{
    isInEpoll_ = false;
}

EventType Handler::get_handler_event()
{
    return event_type_;
}

void Handler::handle_event()
{
    callback_(handler_fd_);
}

bool Handler::is_in_Epoll()
{
    return isInEpoll_;
}
