#pragma once
#include <memory>

template <class T>
class Singleton
{
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
    Singleton(Singleton&&) = delete;
    Singleton& operator=(Singleton&&) = delete;

    typedef std::unique_ptr<T> singleton_pointer_type;
    inline static T* createInstance() { return new T(); }
    inline static T& getReference(const singleton_pointer_type& ptr) { return *ptr; }

protected:
    Singleton() {}

public:
    static T& singleton()
    {
        static typename T::singleton_pointer_type s_singleton(T::createInstance());
        return getReference(s_singleton);
    }

};