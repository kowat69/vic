#pragma once
#include "stdafx.hpp"

namespace my{
template<class T>
class oblate{// pointer隠し
    public:
    inline oblate<T>(){
    }
    inline oblate<T>(T ptr){
        alloc();
        *m_ptr = ptr;
    }
    inline oblate<T>(const oblate<T>& b){
        m_ptr = b.m_ptr;
    }
    inline ~oblate(){}

    static oblate<T> nullopt(){
        oblate<T> t;
        t.m_ptr = 0;
        return t;//pointer
    }

    inline void alloc(){
        m_ptr = new T;
    }
    inline void free(){
        delete m_ptr;
    }

    oblate<T>& operator = (const oblate<T>& b){
        m_ptr = b.m_ptr;
        return *this;
    }

    inline std::optional<T> value_or() const{
        if(m_ptr == 0){
            return std::nullopt;
        }
        return *m_ptr;
    }
    inline bool has_value() const{
        if(m_ptr == 0){
            return false;
        }
        return true;
    }
    inline T& value() const {
        return *m_ptr;
    }
private:
    T* m_ptr = 0;

};
}


