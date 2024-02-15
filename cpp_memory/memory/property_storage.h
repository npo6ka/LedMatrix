#pragma once

#include "vector"
#include "stdint.h"

class IProperty;

class PropertyStorage {
    std::vector<IProperty *> props;

    // singlton property
    // Конструктор копирования и оператор присваивания копированием недоступны
    PropertyStorage() {};
    PropertyStorage(const PropertyStorage& )  = delete;
    PropertyStorage& operator=(const PropertyStorage& ) = delete;
public:
    static PropertyStorage& instance() {
        static PropertyStorage instance;
        return instance;
    }

    void add_property(IProperty *prop);
    void load_all_propertyes();
    void save_all_propertyes();
    void clear();
    uint16_t size();
};
