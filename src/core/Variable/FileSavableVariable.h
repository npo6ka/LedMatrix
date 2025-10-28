#pragma once

#include "Variable.h"
#include "core/common_interfaces/ISaveable.h"
#include "core/Serializer.h"
#include "core/file/IFileSavable.h"

template<typename T>
class FileSavableVariable: public Variable<T>, public ISaveable, public IFileSaveable
{
public:
    FileSavableVariable() = delete;
    FileSavableVariable(IFileHandler* fileHandler,
                        uint32_t offset = 0,
                        const T& defaultValue = {},
                        bool loadOnCreate = true)
    {
        this->_fileHandler = fileHandler;
        this->_offset = offset;
        if (!loadOnCreate || !load()) {
            this->_value = defaultValue;
            save();
        }
    }

    virtual void set(const T& value) override {
        if (this->_value != value) {
            this->_value = value;
            save();
        }
    }

    virtual bool save() override {
        return Serializer<T>::serialize(this->_value, *this);
    }

    virtual bool load() override {
        return Serializer<T>::deserialize(this->_value, *this);
    }
};