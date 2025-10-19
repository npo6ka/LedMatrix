#pragma once

#include "core/Serializer.h"

template<typename T>
class Variable
{
protected:
    T _value;
public:
    Variable() = default;
    Variable(const T& value): _value(value) {}
    ~Variable() = default;

    virtual const T& get() const {
        return _value;
    }

    virtual void set(const T& value) {
        _value = value;
    }

    virtual size_t size() const {
        return Serializer<T>::template typeSize<T>();
    }
};