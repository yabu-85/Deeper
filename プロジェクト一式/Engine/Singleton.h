#pragma once
#include <memory>

template <class T>
class Singleton
{
    static T* instance_;

    inline static void CreateInstance() {
        if (!instance_) {
            instance_ = new T();
        }
    }

    inline static void DestroyInstance() {
        delete instance_;
        instance_ = nullptr;
    }

protected:
    Singleton() = default;
    virtual ~Singleton() = default;

public:
    inline static T& GetInstance() {
        if (!instance_) CreateInstance(std::forward<Args>(args)...);
        return *instance_;
    }


};

template<class T> T* Singleton<T>::instance_ = nullptr;