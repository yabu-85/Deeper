#pragma once

template <class T>
class Singleton
{
    static T* instance_;

protected:
    Singleton() = default;
    virtual ~Singleton() = default;

public:
    static T& GetInstance();
    static void DestroyInstance();

};

template<class T> T* Singleton<T>::instance_ = nullptr;