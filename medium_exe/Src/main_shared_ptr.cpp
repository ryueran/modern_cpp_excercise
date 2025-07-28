#include <iostream>
#include <memory>
#include <vector>

class SharedPtrExample; // Forward declaration
std::vector<std::shared_ptr<SharedPtrExample>> sharedPtrVec;

class SharedPtrExample: public std::enable_shared_from_this<SharedPtrExample> {
private:
    SharedPtrExample() {
        std::cout << "SharedPtrExample created." << std::endl;
    }
public:
    static std::shared_ptr<SharedPtrExample> create() {
        return std::shared_ptr<SharedPtrExample>(new SharedPtrExample());
    }

    void process() {
        sharedPtrVec.emplace_back(shared_from_this());
    }
};

int main() {
    auto ptr1 = SharedPtrExample::create();
    ptr1->process();
    std::cout << "Number of shared pointers in vector: " << sharedPtrVec.size() << std::endl;
    return 0;
}