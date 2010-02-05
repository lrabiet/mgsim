#ifndef DELEGATE_H
#define DELEGATE_H

#include "simtypes.h"
#include <cassert>

namespace Simulator
{

class Object;

class delegate
{
    void     *m_object;
    Result  (*m_stub)(void*);
    Object* (*m_get)(void*);
    
    template <typename T, Result (T::*TMethod)()>
    static Result method_stub(void* object)
    {
        T* p = static_cast<T*>(object);
        return (p->*TMethod)();
    }

    template <typename T>
    static Object* get_stub(void* object)
    {
        return static_cast<T*>(object);
    }
    
    delegate() {}
public:
    template <typename T, Result (T::*TMethod)()>
    static delegate create(T& object)
    {
        delegate d;
        d.m_object = &object;
        d.m_stub   = &method_stub<T,TMethod>;
        d.m_get    = &get_stub<T>;
        return d;
    }
    
    Result operator()() const
    {
        return (*m_stub)(m_object);
    }
    
    Object* GetObject() const { return m_get(m_object); }   
};    

}

#endif