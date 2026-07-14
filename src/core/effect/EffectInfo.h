# pragma once

#include "core/common_interfaces/ISerializable.h"

#include <stdint.h>

class EffectInfo : public ISerializable
{
public:
    uint32_t id;
    uint32_t savedIndex;

    EffectInfo() : id(0), savedIndex(0) {}
    EffectInfo(uint32_t id, uint32_t savedIndex) : id(id), savedIndex(savedIndex) {}

    virtual bool serialize(IStream& stream) const override {
        return stream.write(&id, sizeof(id), 0) && stream.write(&savedIndex, sizeof(savedIndex), sizeof(id));
    }

    virtual bool deserialize(const IStream& stream) override {
        return stream.read(&id, sizeof(id), 0) && stream.read(&savedIndex, sizeof(savedIndex), sizeof(id));
    }

    static size_t typeSize() {
        return sizeof(id) + sizeof(savedIndex);
    }

    bool operator==(const EffectInfo& other) const {
        return id == other.id && savedIndex == other.savedIndex;
    }

    bool operator!=(const EffectInfo& other) const {
        return !(*this == other);
    }

    static const EffectInfo& getErrorEffectInfo() {
        static const EffectInfo errorEffectInfo(0, 0);
        return errorEffectInfo;
    }
};