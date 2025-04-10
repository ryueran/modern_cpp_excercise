#include<iostream>
#include<memory>

/*From this example it is easy to conclude
that if you do not need to create objects based on the
Base class (Factory pattern), you can use CRTP to
implement static polymorphism.
*/
template<typename T>
class Base {
public:
    void interface() {
        static_cast<T*>(this)->implementation();
    }
};

class Derived : public Base<Derived> {
public:
    void implementation() {
        std::cout << "Derived implementation" << std::endl;
    }
};

class Derived2 : public Base<Derived2> {
public:
    void implementation() {
        std::cout << "Derived2 implementation" << std::endl;
    }
};

int main()
{
    std::unique_ptr<Base<Derived>> b = std::make_unique<Derived>();
    std::unique_ptr<Base<Derived2>> d = std::make_unique<Derived2>();
    d->interface(); // Calls Derived::implementation
    b->interface(); // Calls Derived2::implementation
    return 0;
}