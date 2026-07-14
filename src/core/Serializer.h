#pragma once

#include "core/common_interfaces/ISerializable.h"
#include "core/common_interfaces/IStream.h"

#include <type_traits>
#include <cstddef>
#include <functional>

template <typename T>
class Serializer
{
public:
    ~Serializer() = default;

    // For simple types (int, float, etc.)
    template<typename SimpleType>
    static typename std::enable_if<std::is_arithmetic<SimpleType>::value, bool>::type
    serialize(const SimpleType& value, IStream& stream) {
        return stream.write(&value, sizeof(SimpleType));
    }

    template<typename SimpleType>
    static typename std::enable_if<std::is_arithmetic<SimpleType>::value, bool>::type
    deserialize(SimpleType& value, const IStream& stream) {
        return stream.read(&value, sizeof(SimpleType));
    }

    template<typename SimpleType>
    static typename std::enable_if<std::is_arithmetic<SimpleType>::value, size_t>::type
    typeSize() {
        return sizeof(SimpleType);
    }


    // For types derived from ISerializable
    template<typename CustomType>
    static typename std::enable_if<std::is_base_of<ISerializable, CustomType>::value, bool>::type
    serialize(const CustomType& value, IStream& stream) {
        return static_cast<const CustomType&>(value).serialize(stream);
    }

    template<typename CustomType>
    static typename std::enable_if<std::is_base_of<ISerializable, CustomType>::value, bool>::type
    deserialize(CustomType& value, const IStream& stream) {
        return static_cast<CustomType&>(value).deserialize(stream);
    }

    template<typename CustomType>
    static typename std::enable_if<std::is_base_of<ISerializable, CustomType>::value, size_t>::type
    typeSize() {
        return CustomType::typeSize();
    }
};
