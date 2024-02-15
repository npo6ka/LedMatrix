#include "property_storage.h"
#include "property.h"

void PropertyStorage::add_property(IProperty *prop) {
    if (props.size()) {
        prop->set_offset(size());
    } else {
        prop->set_offset(0);
    }

    props.push_back(prop);
}

void PropertyStorage::load_all_propertyes() {
    for (auto &it: props) {
        it->load();
    }
}

void PropertyStorage::save_all_propertyes() {
    for (auto &it: props) {
        it->save();
    }
}

void PropertyStorage::clear() {
    if (props.size()) {
        props.clear();
    }
}

uint16_t PropertyStorage::size() {
    IProperty* prop = props.back();
    return prop->get_offset() + prop->size();
}