#include <HandlerFactory.h>
#include <Reactor.h>
#include <TimerHandler.h>
#include <CounterHandler.h>
#include <iostream>

int main()
{
    // Create a Reactor instance
    Reactor reactor;
    HandlerFactory factory;
    int seconds = 5;
    int milliseconds = 500;
    int count = 10;
    // Create a TimerHandler with a callback that prints the time
    auto timerHandler = factory.createHandler<TimerHandler, int, int>(std::function<void(int, int)>([](int seconds, int milliseconds) -> void {
        std::cout << "Timer triggered after " << seconds << " seconds and " << milliseconds << " milliseconds." << std::endl;
    }));
    // Create a CounterHandler with a callback that prints the count
    auto counterHandler = factory.createHandler<CounterHandler, int>(std::function<void(int)>([](int count) {
        std::cout << "Counter: " << count << std::endl;
    }));
    timerHandler->indreickVoker(seconds, milliseconds); // Invoke the TimerHandler with specific arguments
    counterHandler->indreickVoker(count); // Invoke the CounterHandler with specific arguments
    // Register the TimerHandler with the Reactor
    reactor.registerHandler("TimerHandler", std::move(timerHandler));
    
    // Register the CounterHandler with the Reactor
    reactor.registerHandler("CounterHandler", std::move(counterHandler));
    
    // Run the handlers
    reactor.runHandlers("TimerHandler");
    reactor.runHandlers("CounterHandler");
    // Unregister the handlers
    reactor.unregisterHandler("TimerHandler");
    reactor.unregisterHandler("CounterHandler");
    return 0;
}