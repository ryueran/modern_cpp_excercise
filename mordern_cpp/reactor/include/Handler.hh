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
        delete message_buff_;
    }

    void enable_write();
    void enable_read();
    void enable_close();
    void setReadCallback(CallBack read_callback);
    void setWriteCallback(CallBack write_callback);
    void setCloseCallback(CallBack close_callback);
private:
    int handler_fd_;
    int event_type_;
    bool isInEpoll_;
    char message_buff_[BUFFER_SIZE];
};

#endif