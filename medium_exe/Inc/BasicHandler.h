#ifndef BASIC_HANDLER_H
#define BASIC_HANDLER_H


class BasicHandler
{
    // Base class for handlers, providing a common interface
    // for derived classes to implement specific handling logic.
public:
    BasicHandler() = default;
    virtual ~BasicHandler() = default;
    virtual void handle() = 0; // Pure virtual function to be implemented by derived classes
};

#endif