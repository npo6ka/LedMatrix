/*#pragma once

#include "vector"
#include "stdint.h"

class IProperty;

class PropertyStorage {
    // singlton property
    // Конструктор копирования и оператор присваивания копированием недоступны
    PropertyStorage() = default;
    ~PropertyStorage() = default;
    PropertyStorage(const PropertyStorage& )  = delete;
    PropertyStorage& operator=(const PropertyStorage& ) = delete;
public:
    static PropertyStorage& instance() {
        static PropertyStorage instance;
        return instance;
    }

    uint16_t add_property(IProperty *prop);
    std::vector<IProperty *> get_props() const;

    void load_all_propertyes();
    void save_all_propertyes();
    void clear();
    uint16_t size();
private:
    std::vector<IProperty *> _props;
};*/
