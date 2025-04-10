#include <iostream>

template<typename T>
T sum(T t)
{
    return t;
}

template<typename T, typename... Args>
T sum(T first, Args... args)
{
    return first + sum<T>(args...);
}

int main()
{
    std::cout << sum<int>(1, 2, 3, 4) << std::endl;
    return 0;
}