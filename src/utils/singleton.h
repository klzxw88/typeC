#ifndef SINGLETON_H  
#define SINGLETON_H  

#include <memory>

using namespace std;

template<typename T>
class Singleton {
protected:
    Singleton() {}
    static T* _instance;
public:
    Singleton(const Singleton& c) = delete;
    void operator=(const Singleton& c) = delete;

    static T* instance();
};

template<typename T>
T* Singleton<T>::instance() {
    if (_instance == nullptr) {
        _instance = new T();
    }
    return _instance;
}

template <typename T>
T* Singleton<T>::_instance = nullptr;

#endif  
