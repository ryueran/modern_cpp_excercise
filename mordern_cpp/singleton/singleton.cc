#include <iostream>

class Singleton
{
public:
    static Singleton& getInstance() {
        static Singleton instance; // local static variable thread safe (from c++11)
        return instance;
    }

    void printAddress() {
        std::cout << "Instance Address: " << this << std::endl;
    }

    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
private:
    Singleton() {}
};

int main()
{
    Singleton::getInstance().printAddress();
    return 0;
}