#include <iostream>
#include <string>

class Widget {
public:
    explicit Widget(int w, int l): width_(w), length_(l)
    {
        std::cout << "Widget constructed!" << std::endl;
    }

    Widget(const Widget& widget)
    {
        std::cout << "Widget copy constructor!" << std::endl;
        width_ = widget.width_;
        length_ = widget.length_;
    }

    Widget(Widget&& widget)
    {
        std::cout << "Widget move constructor!" << std::endl;
        width_ = widget.width_;
        length_ = widget.length_;
        widget.width_ = 0;
        widget.length_ = 0;
    }
    int width_;
    int length_;
};

void processWidget(const Widget& w)
{
    std::cout << "copy process widget!" << std::endl;
    std::cout << w.width_ + w.length_ << std::endl;
}

void processWidget(Widget&& w)
{
    std::cout << "move process widget!" << std::endl;
    std::cout << w.width_ * w.length_ << std::endl;
}

template<typename T>
void longProcess(T&& param)
{
    processWidget(std::forward<T>(param));
}

int main()
{
    Widget w(10, 11);
    longProcess(w);
    longProcess(std::move(w));
    return 0;
}