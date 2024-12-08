#include <iostream>

template<typename T, std::size_t N>
constexpr std::size_t arraySize(T(&)[N]) noexcept
{
    return N;
}

int main()
{
    char str[] = "abcdefg";
    std::cout << arraySize(str) << std::endl;
    return 0;
}