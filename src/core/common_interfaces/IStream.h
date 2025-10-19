#pragma once

#include <cstddef>

class IStream
{
public:
    virtual ~IStream() = default;

    virtual bool write(const void* data, size_t size, size_t seekOffset = 0) = 0;
    virtual bool read(void* data, size_t size, size_t seekOffset = 0) const = 0;
};
