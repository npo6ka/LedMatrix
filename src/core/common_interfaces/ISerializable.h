#pragma once

#include "core/common_interfaces/IStream.h"

class ISerializable
{
public:
    virtual ~ISerializable() = default;

    virtual bool serialize(IStream& stream) const = 0;
    virtual bool deserialize(const IStream& stream) = 0;
};