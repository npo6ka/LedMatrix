#pragma once

#include <Vector.h>

class IProperty;

class PropertyStorage {
    std::vector<IProperty*> items;
    uint16_t addr_offset = -1;

    PropertyStorage() = default;
    ~PropertyStorage() = default;

    PropertyStorage(const PropertyStorage&) = delete;
    PropertyStorage(PropertyStorage&&) = delete;

    PropertyStorage& operator=(const PropertyStorage&) = delete;
    PropertyStorage& operator=(PropertyStorage&&) = delete;

public:
    static PropertyStorage& instance()
    {
        static PropertyStorage obj;
        return obj;
    }

    void set_offset(uint16_t offset);
    uint16_t get_offset();
    uint8_t get_mod_size();

    void add(IProperty* item);
    void clear(IProperty* item);

    void save() const;
    void load();
};