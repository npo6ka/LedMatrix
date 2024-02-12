#include "property.h"

void PropertyStorage::set_offset(uint16_t offset) {
    addr_offset = offset;
}
uint16_t PropertyStorage::get_offset() {
    return addr_offset;
}

uint8_t PropertyStorage::get_mod_size() {
    uint32_t size = 0;

    for (auto it : items) {
        size += it->size();
    }

    if (size > 0xff) {
        out("ERROR: Mod size is very big. More then 255\n");
        return 0;
    }
    return size;
}

void PropertyStorage::add(IProperty* item)
{ items.push_back(item); }

void PropertyStorage::clear(IProperty* item)
{
    if (items.size() > 0) {
        items.clear();
    }
}

void PropertyStorage::load()
{
    for (auto it : items) {
        it->load();
    }
}

void PropertyStorage::save() const
{
    for (auto it : items) {
        it->save();
    }
}