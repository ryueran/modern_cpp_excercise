#include <iostream>
#include <memory>
#include <functional>
#include <map>
#include <boost/noncopyable.hpp>

class Object
{
public:
    Object(const std::string& key): key_(key) {std::cout << "Object constructed!" << std::endl;}
    ~Object() {std::cout << "Object destrcuted!" << std::endl;}
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
    std::shared_ptr<T> get(const std::string& key)
    {
        std::shared_ptr<T> sPtr;
        std::weak_ptr<T>& wPtr = factory_[key];
        sPtr = wPtr.lock();
        if(!sPtr)
        {
            sPtr.reset(
                new Object(key), 
                std::bind(
                    &ObjectFactory::WeakDeleteCallback, 
                    std::weak_ptr<ObjectFactory<T>>(std::enable_shared_from_this<ObjectFactory<T>>::shared_from_this()), 
                    std::placeholders::_1
                )
            );
            wPtr = sPtr;
        }
        return sPtr;
    }
private:
    static void WeakDeleteCallback(std::weak_ptr<ObjectFactory<T>> wPtr, T *obj)
    {
        std::shared_ptr<ObjectFactory<T>> sPtr = wPtr.lock();
        if(sPtr)
        {
            sPtr->deleteObj(obj);
        }
        delete obj;
    }

    void deleteObj(T *obj)
    {
        if(obj)
        {
            factory_.erase(obj->key());
        }
    }

    std::map<std::string, std::weak_ptr<Object>> factory_;
};

int main()
{
    std::shared_ptr<ObjectFactory<Object>> factory = std::make_shared<ObjectFactory<Object>>();
    std::shared_ptr<Object> obj = factory->get("IBN: 47877");
    std::cout << obj.use_count() << std::endl;
    return 0;
}