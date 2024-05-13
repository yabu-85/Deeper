#pragma once
#include <memory>

template <class T>
class Singleton
{
    static std::unique_ptr<T> instance_;

protected:
    Singleton() = default;
    virtual ~Singleton() = default;

public:
    //�R�s�[�֎~
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    //���[�u�֎~
    Singleton(Singleton&&) = delete;
    Singleton& operator=(Singleton&&) = delete;

    static std::unique_ptr<T>& GetInstance()
    {
        if (!instance_)
        {
            struct A : T {};
            instance_ = std::make_unique<A>();
        }

        return instance_;
    }
    static void DestroyInstance()
    {
        instance_.reset();
        instance_ = nullptr;
    }

};