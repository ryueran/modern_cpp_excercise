#include <iostream>
#include <boost/noncopyable.hpp>
#include <memory>
#include <string>
#include <map>
#include <functional>

class Object
{
public:
    Object(const std::string& key): key_(key) {std::cout << "Object constructed!" << std::endl;}
    ~Object() {std::cout << "Object destructed!" << std::endl;}
    std::string key()
    {
        return key_;
    }
private:
    std::string key_;
};

template<typename T>
class ObjectFactory: public boost::noncopyable, public std::enable_shared_from_this<ObjectFactory<T>>
{
public:
    std::shared_ptr<T> get(const std::string& key);
private:
    static void WeakDelCallback(std::weak_ptr<ObjectFactory<T>> wPtr, T *object)
    {
        std::shared_ptr<ObjectFactory<T>> sPtr = wPtr.lock();
        if(sPtr)
        {
            sPtr->deleteObject(object);
        }
        delete object;
    }

    void deleteObject(T *object)
    {
        if(object)
        {
            factory_.erase(object->key());
        }
    }

    std::map<std::string, std::weak_ptr<T>> factory_;
};

template<typename T>
std::shared_ptr<T> ObjectFactory<T>::get(const std::string& key) {
    std::shared_ptr<T> pObj;
    std::weak_ptr<T>& wObj = factory_[key];
    pObj = wObj.lock();
    if(!pObj) {
        std::cout << "Create new shared pointer!" << std::endl;
        pObj.reset(new T(key), std::bind(&ObjectFactory<T>::WeakDelCallback, 
            std::weak_ptr<ObjectFactory<T>>(std::enable_shared_from_this<ObjectFactory<T>>::shared_from_this()), 
            std::placeholders::_1)); // to avoid that Factory is deconstructed before Object
        wObj = pObj;
    }
    return pObj;
}

int main()
{
    std::shared_ptr<ObjectFactory<Object>> factory = std::make_shared<ObjectFactory<Object>>();
    std::shared_ptr<Object> pObj = factory->get("Lego");
    std::cout << pObj.use_count() << std::endl;
    return 0;
}
