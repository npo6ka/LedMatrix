#pragma once

#include "debug_lib.h"
#include "property_storage.h"

class ISaveable {
public:
    virtual ~ISaveable() = default;
    virtual void save() const = 0;
    virtual void load() = 0;
};

class IProperty : ISaveable {
public:
    virtual ~IProperty() = default;
    virtual void save() const = 0;
    virtual void load() = 0;
    virtual uint16_t size() const = 0;
};

template<class T>
class Property : IProperty {
    T m_value;
    T min_val;
    T max_val;

public:
    Property(T value, T min, T max) : m_value(value), min_val(min), max_val(max)
    {
        PropertyStorage::instance().add(this);
    }
    Property(T min, T max) : Property({}, min, max) { }
    Property(T val) : Property(val, {}, {}) { }
    Property() : Property({}, {}, {}) { }

    ~Property()
    {
        PropertyStorage::instance().clear(this);
    }

    Property(const Property&) = delete;
    Property(Property&&) = delete;

    Property& operator=(const Property&) = delete;
    Property& operator=(Property&&) = delete;

    Property& operator=(T value)
    {
        m_value = value;
        return *this;
    }

    operator T() const
    { return m_value; }

    T& operator*()
    { return m_value; }

    const T& operator*() const
    { return m_value; }

    T& operator->()
    { return m_value; }

    const T& operator->() const
    { return m_value; }

    void save() const override
    {}

    void load() override
    {}

    uint16_t size() const override
    {
        return sizeof(T);
    }

    T get_min()
    {
        return min_val;
    }

    T get_max()
    {
        return max_val;
    }
};
