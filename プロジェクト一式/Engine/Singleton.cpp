#include "Singleton.h"

template<class T>
void Singleton<T>::DestroyInstance()
{
    delete instance_;
    instance_ = nullptr;
}

template<class T>
T& Singleton<T>::GetInstance()
{
    if (!instance_) {
        instance_ = new T();
    }

    return *instance_;
}