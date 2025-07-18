#ifndef HANDLERFACTORY_H
#define HANDLERFACTORY_H

#include "Handler.h"
#include <functional>
#include <type_traits>
#include <memory>
#include <map>

class HandlerFactory {
public:
    template<typename HandlerType, typename... Args>
    std::unique_ptr<Handler<Args...>> createHandler(std::function<void(Args...)> callback) {
        auto handler = std::make_unique<HandlerType>(callback);
        return handler;
    }
};

#endif