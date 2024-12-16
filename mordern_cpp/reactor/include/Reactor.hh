#ifndef REACTOR_HH
#define REACTOR_HH

#include <iostream>
#include <vector>
#include <memory>
#include "Handler.hh"

class Reactor
{
public:
    Reactor() {};
    ~Reactor() {};
    void register_handler(std::shared_ptr<Handler>& ptr_handler);
    void remove_handler();
    void loop();
private:
    std::vector<std::shared_ptr<Handler>> handlers_;
};

#endif