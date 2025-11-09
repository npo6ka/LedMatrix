# pragma once

#include "core/Variable/IVariable.h"
#include "core/file/IFileHandler.h"

#include <vector>
#include <functional>
#include <memory>

class IEffectVariableStorage
{
public:
    virtual ~IEffectVariableStorage() = default;

    virtual IVariable& addVariable(std::unique_ptr<IVariable>&& variable) = 0;
    virtual void clear() = 0;
    virtual const std::vector<std::unique_ptr<IVariable>>& getVariables() const = 0;

    // функции для работы с файлами
    virtual IFileHandler* getFileHandler() { return nullptr; }
    virtual uint32_t getCurrentOffset() const { return 0; }
};
