#ifndef REACTOR_HH
#define REACTOR_HH

#include <iostream>
#include <vector>
#include <memory>
#include "Handler.hh"
#include "Epoller.hh"

class Reactor
{
public:
    Reactor(int fd) {ptr_epoller.reset(new Epoller(fd));}
    ~Reactor() {std::cout << "Reactor destructed!" << std::endl;}
    void register_handler(Handler* ptr_handler);
    void remove_handler(Handler* ptr_handler);
    void loop();
private:
    std::unique_ptr<Epoller> ptr_epoller;
};

#endif