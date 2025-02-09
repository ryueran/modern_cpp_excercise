#ifndef HANDLER_HH
#define HANDLER_HH

#include <iostream>
#include <functional>

#define BUFFER_SIZE 1024

using CallBack = std::function<void(int)>;

enum class EventType {Read, Write, Close};

class Handler
{
private:
    int handler_fd_;
    bool isInEpoll_;
    // char message_buff_[BUFFER_SIZE];
    CallBack callback_;

public:
    EventType event_type_;
    Handler(int fd): handler_fd_(fd) {
        std::cout << "Construct Handler!" << std::endl;
        isInEpoll_ = false;
    }

    ~Handler()
    {
        std::cout << "Destructed Handler!" << std::endl;
        // delete message_buff_;
    }

    void setCallback(CallBack callback);
    int get_handler_fd();
    void handle_event();
    void set_in_Epoll();
    void set_out_Epoll();
    bool is_in_Epoll();
    EventType get_handler_event();
    char message_buffer[1024];
};

#endif