#pragma once

#include "property_storage.h"
#include "memory_manager.h"
#include "stdint.h"

// костыль для получения имени типа без использования RTTI
namespace type_names {
    template<typename T>
    inline const char* get_type_name() { return "unknown"; }

    #define DEFINE_TYPE_NAME(type) \
    template<> \
    inline const char* get_type_name<type>() { return #type; }

    DEFINE_TYPE_NAME(uint8_t);
    DEFINE_TYPE_NAME(uint16_t);
    DEFINE_TYPE_NAME(uint32_t);
    DEFINE_TYPE_NAME(uint64_t);
    DEFINE_TYPE_NAME(int8_t);
    DEFINE_TYPE_NAME(int16_t);
    DEFINE_TYPE_NAME(int32_t);
    DEFINE_TYPE_NAME(int64_t);
    DEFINE_TYPE_NAME(float);
    DEFINE_TYPE_NAME(double);
    DEFINE_TYPE_NAME(bool);
}

class ISaveable {
    uint16_t offset = -1;
public:
    virtual ~ISaveable() = default;
    virtual void save() = 0;
    virtual void load() = 0;
    virtual void set_offset(uint16_t val) {
        offset = val;
    }
    virtual uint16_t get_offset() {
        return offset;
    }
    virtual size_t size() const = 0;
};

class IProperty : public ISaveable {
public:
    virtual ~IProperty();
    virtual const char* type_name() const = 0;
    virtual void save() override = 0;
    virtual void load() override = 0;
    virtual size_t size() const override = 0;
};

template<class T>
class Property : public IProperty {
    Property(const Property&) = delete;
    Property(Property&&) = delete;
    Property& operator=(const Property&) = delete;
    Property& operator=(Property&&) = delete;

protected:
    virtual void set_without_save(T new_val) {
        if ((_min_value == _max_value) || // не определены занчения min max
            (new_val >= _min_value && new_val <= _max_value)) // новое значение попадает в диапозон
        {
            _value = new_val;
        }
    }

public:
    Property(T value, T min, T max): _value(), _min_value(min), _max_value(max) {
        set_without_save(value);
        uint16_t offset = PropertyStorage::instance().add_property(this);
        set_offset(offset);
    }
    Property(T min, T max) : Property({}, min, max) { }
    Property(T val) : Property(val, {}, {}) { }
    Property() : Property({}, {}, {}) { }

    virtual ~Property() {
        PropertyStorage::instance().clear();
    }

    T get() {
        return _value;
    }

    void set(T new_val) {
        if (_value != new_val) {
            set_without_save(new_val);
            save();
        }
    }

    void save() override {
        MemoryManager::instance().save_mod_var(get_offset(), _value);
    }

    void load() override {
        MemoryManager::instance().load_mod_var(get_offset(), _value);
    }

    size_t size() const override {
        return sizeof(T);
    }

    const char* type_name() const override {
        return type_names::get_type_name<T>();
    }

    bool has_min_max() const {
        return _min_value != _max_value;
    }

    T get_min() const {
        return _min_value;
    }

    T get_max() const {
        return _max_value;
    }

private:
    T _value;
    T _min_value;
    T _max_value;
};