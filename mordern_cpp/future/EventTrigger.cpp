#include <iostream>
#include <future>
#include <map>
#include <chrono>
#include <thread>
#include <vector>
#include <mutex>

class EventPoller {
public:
    using EventID = int;

    // 注册事件，返回 future
    std::future<void> registerEvent(EventID id) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = events_.find(id);
        if(it == events_.end())
        {
            std::unique_ptr<std::promise<void>> event = std::make_unique<std::promise<void>>();
            std::future<void> future = event->get_future();
            events_[id] = std::move(event);
            return future; 
        } else {
            throw std::runtime_error("Event already registered!");
        }
    }

    // 触发事件
    void triggerEvent(EventID id) {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = events_.find(id);
        if(it != events_.end())
        {
            it->second->set_value();
            events_.erase(id);
        }
    }

private:
    std::map<EventID, std::unique_ptr<std::promise<void>>> events_;
    std::mutex mutex_;
};

// 测试代码
int main() {
    EventPoller poller;
    auto future1 = poller.registerEvent(1);
    std::thread t1 ([&](){
        std::this_thread::sleep_for(std::chrono::seconds(2));
        poller.triggerEvent(1);
    });
    future1.wait();
    std::cout << "Main thread: Event 1 handled." << std::endl;
    t1.join();
    return 0;
}
