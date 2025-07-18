#ifndef     COUNTER_HANDLER_H
#define     COUNTER_HANDLER_H

#include "Handler.h"
#include <iostream>
class CounterHandler : public Handler<int> {
public:
    CounterHandler(std::function<void(int)> callback)
        : Handler<int>(callback) {}
    ~CounterHandler() override {
        callback_ = nullptr; // Clear the callback when the handler is destroyed
        std::cout << "CounterHandler destroyed" << std::endl;
    }
};

#endif