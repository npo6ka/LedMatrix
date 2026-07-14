#pragma once

#include "core/common_interfaces/IStream.h"

#include <cstdint>
#include <cstddef>

class IFileHandler : public IStream
{
public:
    virtual ~IFileHandler() = default;

    virtual void open(const char* path) = 0;
    virtual void close() = 0;

    virtual bool write(const void* data, size_t size, size_t seekOffset = 0) override = 0;
    virtual bool read(void* data, size_t size, size_t seekOffset = 0) const override = 0;
    virtual bool seek(size_t position) = 0;
    virtual size_t size() const = 0;
};