#include <iostream>
#include <initializer_list>

template<typename T>
T sum(std::initializer_list<T> nums)
{
    int total = 0;
    for(auto i : nums)
    {
        total += i;
    }
    return total;
}

int main()
{
    int a = sum<int>({1, 2, 3});
    std::cout << a << std::endl;
    return 0;
}