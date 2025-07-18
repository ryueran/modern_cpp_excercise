#ifndef HANDLER_H
#define HANDLER_H

#include "BasicHandler.h"
#include <functional>

template<typename... Args>
class Handler: public BasicHandler {
    // Handler class that inherits from BaiscHandler
    // It can take any number of arguments and has a callback function
    // that can be invoked with those arguments.
public:
    Handler(std::function<void(Args...)> callback): callback_(std::move(callback)) {}
    virtual ~Handler() = default;
    Handler& operator=(const Handler& other) // Disable copy assignment
    {
        if (this != &other) {
            callback_ = other.callback_; // Copy the callback function
        }
        return *this; // Return the current instance
        // Assignment operator is not defined, as handlers are not meant to be copie
    }
    void handle() {
        if (callback_) {
            callback_(Args{}...); // Call the callback with default-constructed arguments
        }
    }

    void indreickVoker(Args... args) {
        if (callback_) {
            callback_(args...); // Invoke the callback with provided arguments
        }
    }

    std::function<void(Args...)> callback_; // Example member variable to hold a callback function
};
#endif