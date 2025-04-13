#include <iostream>

template<typename T>
T sum()
{
    return 0;
}

template<typename T, auto arg, auto... args>
T sum()
{
    return arg + sum<T, args...>();
}

int main()
{
    std::cout << sum<int, 0, 1, 2, 3, 4>() << std::endl;
    return 0;
}