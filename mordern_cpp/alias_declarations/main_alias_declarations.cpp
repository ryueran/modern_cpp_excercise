#include <iostream>
#include <list>
#include <string>

template<typename T>
struct MyAlloc {
    using value_type = T;
    MyAlloc() = default;
    template<typename U>
    MyAlloc(const MyAlloc<U>&) {}
    T* allocate(std::size_t n) {return static_cast<T*>(::operator new(n * sizeof(T)));}
    void deallocate(T *p, std::size_t n) {::operator delete(p);}
};

template<typename T>
using MyAllocList = std::list<T, MyAlloc<T>>;

template<typename T>
class Widget
{
public:
    void add(const T& item)
    {
        list.push_back(item);
    }

    void output()
    {
        for(auto item : list)
        {
            std::cout << item << std::endl;
        }
    }
private:
    MyAllocList<T> list; 
};

int main()
{
    Widget<std::string> string_w;
    string_w.add("Hello ");
    string_w.add("World");
    string_w.output();
    return 0;
}