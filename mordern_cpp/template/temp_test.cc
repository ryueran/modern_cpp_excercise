#include <functional>
#include <type_traits>
#include <iostream>

template<typename R, typename... Args>
class MyDelegate
{
public:
    MyDelegate(R f(Args...)): dele_function_(f) {}

    R operator() (Args&&... args)
    {
        return dele_function_(std::forward<Args>(args)...);
    }
private:
    std::function<R(Args...)> dele_function_;
};

template<typename R, typename... Args>
MyDelegate<R, Args...> CreateDelegate(R f(Args...))
{
    return MyDelegate<R, Args...> (f);
}


void Fun(int i) {std::cout << i << std::endl;}
void Fun1(int i, int j) {std::cout << i + j << std::endl;}

int main()
{
    auto d = CreateDelegate(Fun);
    d(1);
    auto d1 = CreateDelegate(Fun1);
    d1(1, 2);
    return 0;
}