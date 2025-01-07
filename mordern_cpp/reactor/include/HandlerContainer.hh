#ifndef HANDLERCONTAINER_HH
#define HANDLERCONTAINER_HH

#include <list>
#include "Handler.hh"

template<typename T>
struct MyAlloc {
    using value_type = T;
    MyAlloc() = default;
    template<typename U>
    MyAlloc(const MyAlloc<T>&) {}
    T* allocate(std::size_t n) {return static_cast<T*>(::operator new(n * sizeof(T)));}
    void deallocate(T *p, size_t n) {::operator delete(p);}
};

template<typename T>
using HandlerList = std::list<T, MyAlloc<T>>;

template<typename T>
class HandlerContainer
{
public:
    HandlerList<T> list;
    void add(T t)
    {
        list.push_back(t);
    }
};

#endif