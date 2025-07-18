#ifndef TIMER_HANDLER_H
#define TIMER_HANDLER_H

#include "Handler.h"
#include <functional>
#include <iostream>

class TimerHandler: public Handler<int, int> {
public:
    TimerHandler(std::function<void(int, int)> callback)
        : Handler<int, int>(callback) {}

    ~ TimerHandler() override
    {
        callback_ = nullptr; // Clear the callback when the handler is destroyed
        std::cout << "TimerHandler destroyed" << std::endl;
    }
};

#endif