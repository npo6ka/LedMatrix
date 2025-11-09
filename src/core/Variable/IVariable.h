#pragma once

#include <string>

class IVariable
{
public:
    IVariable() = default;
    ~IVariable() = default;

    virtual const void* getRaw() const = 0;
    virtual void setRaw(void* value) = 0;
    virtual std::string getName() const = 0;
    virtual size_t size() const = 0;
};