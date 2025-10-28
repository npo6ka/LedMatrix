/*#include "property_storage.h"
#include "property.h"

uint16_t PropertyStorage::add_property(IProperty *prop) {
    uint16_t offset = 0;
    if (_props.size()) {
        offset = size();
    }

    _props.push_back(prop);

    return offset;
}

std::vector<IProperty *> PropertyStorage::get_props() const {
    return _props;
}

void PropertyStorage::load_all_propertyes() {
    for (auto &it: _props) {
        it->load();
    }
}

void PropertyStorage::save_all_propertyes() {
    for (auto &it: _props) {
        it->save();
    }
}

void PropertyStorage::clear() {
    if (_props.size()) {
        _props.clear();
    }
}

uint16_t PropertyStorage::size() {
    IProperty* prop = _props.back();
    return prop->get_offset() + prop->size();
}*/