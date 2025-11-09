#pragma once

#include "IEffectVariableStorage.h"

#include "core/file/LsfFileHandler.h"
#include "core/Variable/FileSavableVariable.h"
#include "libs/StdFeatures.h"

#include <vector>
#include <memory>

class FileEffectVariableStorage : public IEffectVariableStorage
{
    LsfFileHandler _storage;
    std::vector<std::unique_ptr<IVariable>> _variables;
    std::string _filePath;
    bool _isOpened = false;
    uint32_t _offset = 0;

public:
    FileEffectVariableStorage(const std::string& filePath) : _filePath(filePath) {}

    virtual ~FileEffectVariableStorage() = default;

    virtual IFileHandler* getFileHandler() override {
        // лениво открываем файл
        if (!_isOpened) {
            _storage.open(_filePath.c_str());
            _isOpened = true;
        }

        return &_storage;
    }

    virtual uint32_t getCurrentOffset() const override {
        return _offset;
    }

    virtual IVariable& addVariable(std::unique_ptr<IVariable>&& variable) override {
        _offset += variable->size();
        _variables.push_back(std::move(variable));
        return *_variables.back();
    }

    virtual void clear() override {
        _offset = 0;
        _variables.clear();
    }

    virtual const std::vector<std::unique_ptr<IVariable>>& getVariables() const override {
        return _variables;
    }
};