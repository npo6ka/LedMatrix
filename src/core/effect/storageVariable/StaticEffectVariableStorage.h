#pragma once

#include "IEffectVariableStorage.h"
#include "core/Variable/Variable.h"

#include <vector>
#include <memory>

class StaticEffectVariableStorage : public IEffectVariableStorage
{
    std::vector<std::unique_ptr<IVariable>> _variables;
    uint32_t _offset = 0;

public:
    StaticEffectVariableStorage() = default;
    virtual ~StaticEffectVariableStorage() = default;

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

