#include <iostream>
#include <boost/noncopyable.hpp>
#include <memory>
#include <string>
#include <map>

class Object
{
public:
    Object() {std::cout << "Object constructed!" << std::endl;}
    ~Object() {std::cout << "Object destructed!" << std::endl;}
};

template<typename T>
class ObjectFactory: public boost::noncopyable
{
public:
    std::shared_ptr<T> get(const std::string& key);
private:
    std::map<std::string, std::weak_ptr<T>> factory_;
};

template<typename T>
std::shared_ptr<T> ObjectFactory<T>::get(const std::string& key) {
    std::shared_ptr<T> pObj;
    std::weak_ptr<T>& wObj = factory_[key];
    pObj = wObj.lock();
    if(!pObj) {
        std::cout << "Create new shared pointer!" << std::endl;
        pObj = std::make_shared<T>();
        wObj = pObj;
    }
    return pObj;
}

int main()
{
    ObjectFactory<Object> factory;
    std::shared_ptr<Object> pObj = factory.get("Lego");
    std::cout << pObj.use_count() << std::endl;
    return 0;
}
