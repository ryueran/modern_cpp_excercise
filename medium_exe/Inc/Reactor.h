#ifndef REACTOR_H
#define REACTOR_H

#include <iostream>
#include <map>
#include <string>
#include <memory>
#include "BasicHandler.h"

class Reactor {
public:
    Reactor() {}
    ~Reactor(){
        for (auto& pair : handlers_) {
            pair.second.reset(); // Ensure all handlers are cleaned up
        }
    }

    void registerHandler(const std::string& name, std::unique_ptr<BasicHandler> handler) {
        handlers_[name] = std::move(handler);
    }

    void unregisterHandler(const std::string& name) {
        handlers_.erase(name); // Remove the handler from the map
    }

    void runHandlers(const std::string& name) {
        auto it = handlers_.find(name);
        if (it != handlers_.end()) {
            it->second->handle(); // Call the handle method of the registered handler
        } else {
            // Handle the case where the handler is not found
            std::cout << "Handler not found: " << name << std::endl;
        }
    }

private:
    std::map<std::string, std::unique_ptr<BasicHandler>> handlers_; // Map to hold handlers by type name
};

#endif