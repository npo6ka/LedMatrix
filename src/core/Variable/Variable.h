#pragma once

#include "core/Serializer.h"
#include "IVariable.h"
#include "libs/RttiFeature.h"

template<typename T>
class Variable : public IVariable
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

    virtual size_t size() const override {
        return Serializer<T>::template typeSize<T>();
    }

    // функции для работы с переменной по вайфай
    virtual const void* getRaw() const override {
        return reinterpret_cast<const void*>(&_value);
    }

    virtual void setRaw(void* value) override {
        _value = *reinterpret_cast<T*>(value);
    }

    virtual std::string getName() const override {
        return type_name<T>();
    }
};