#include "Widget.hh"

int main()
{
    auto wp = Widget::create<int>(1, 2);
    wp->process();
    return 0;
}