#ifndef HANDLER_HH
#define HANDLER_HH

#include <iostream>
#include <functional>

#define BUFFER_SIZE 1024

using CallBack = std::function<void(int)>;

class Handler
{
public:
    Handler(int fd): handler_fd_(fd) {
        std::cout << "Construct Handler!" << std::endl;
    }

    ~Handler()
    {
        std::cout << "Destructed Handler!" << std::endl;
        // delete message_buff_;
    }

    void enable_write();
    void enable_read();
    void enable_close();
    void setReadCallback(CallBack& re_callback);
    void setWriteCallback(CallBack& wr_callback);
    void setCloseCallback(CallBack& cl_callback);
    int get_handler_fd();
    void handle_event();
    void set_in_Epoll();
private:
    enum class EventType {Read, Write, Close};
    int handler_fd_;
    EventType event_type_;
    bool isInEpoll_;
    // char message_buff_[BUFFER_SIZE];
    CallBack read_callback_;
    CallBack write_callback_;
    CallBack close_callback_;
};

#endif