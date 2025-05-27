#pragma once

#include "property_storage.h"
#include "memory_manager.h"
#include "stdint.h"

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
};

class IProperty : public ISaveable {
public:
    virtual ~IProperty() = default;
    virtual void save() = 0;
    virtual void load() = 0;
    virtual size_t size() const = 0;
};

template<class T>
class Property : public IProperty {
    T m_val;
    T max_val;
    T min_val;

    Property(const Property&) = delete;
    Property(Property&&) = delete;
    Property& operator=(const Property&) = delete;
    Property& operator=(Property&&) = delete;

    // установка значения без сохранения в память.
    // Возвращает: 0 если значение успешно установлено
    // 1 если входное значение равно старому значению
    // 2 если установлен диапозон и значение не попадает в установленный диапозон
    bool set_without_save(T new_val) {
        if (m_val != new_val) { //старое значение не равно новому
            if (min_val == max_val || // не определены занчения min max
                new_val >= min_val && new_val <= max_val) // новое значение попадает в диапозон
            {
                m_val = new_val;
                return 0;
            } else {
                return 2;
            }
        }
        return 1;
    }
public:

    Property(T val, T min, T max): m_val(min), min_val(min), max_val(max) {
        set_without_save(val); // устанавливает новое значение m_val, либо оставляет min_val
        PropertyStorage::instance().add_property(this); // + инициализация offset
        printf("new var\n");
    }
    Property(T min, T max) : Property({}, min, max) { }
    Property(T val) : Property(val, {}, {}) { }
    Property() : Property({}, {}, {}) { }

    ~Property()
    {
        PropertyStorage::instance().clear();
    }

    T get() {
        return m_val;
    }

    void set(T new_val) {
        if (set_without_save(new_val) == 0) {
            save();
        }
    }

    // void *row() {
    //     return &m_val;
    // }

    void save() override {
        MemoryManager::instance().save_mod_var(get_offset(), m_val);
    }

    void load() override {
        T val = {};
        MemoryManager::instance().load_mod_var(get_offset(), val);
        if (set_without_save(val) == 2) {
            out("Error load Property: value out of defined range\n");
        }
    }

    size_t size() const override {
        return sizeof(T);
    }
};