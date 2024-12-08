#ifndef WIDGET_H
#define WIDGET_H
#include <iostream>
#include <memory>
#include <vector>

class Widget:public std::enable_shared_from_this<Widget> {
public:
    ~Widget() {}
    template<typename... TS>
    static std::shared_ptr<Widget> create(TS&&... params)
    {
        // auto widget_ptr = std::make_shared<Widget>(std::forward<TS>(params)...);
        return std::shared_ptr<Widget>(new Widget(std::forward<TS>(params)...));
    }
    void process()
    {
        process_widgets.emplace_back(shared_from_this());
    }

    auto read_process_widget()
    {
        return process_widgets;
    }
private:
    Widget(int h, int w): height_(h), width_(w) {}
    std::vector<std::weak_ptr<Widget>> process_widgets;
    int height_;
    int width_;
};

#endif