#pragma once

#include "core/file/IFileHandler.h"
#include "core/common_interfaces/IStream.h"
#include "libs/debug_lib.h"

#include <cstdint>
#include <cstddef>

class IFileSaveable : public IStream
{
protected:
    IFileHandler* _fileHandler;
    uint32_t _offset;
public:
    virtual ~IFileSaveable() = default;

    virtual bool write(const void* data, size_t size, size_t seekOffset = 0) override {
        if (_fileHandler == nullptr) {
            logError("Error writing file: file handler is not set\n");
            return false;
        }

        if (!_fileHandler->seek(_offset + seekOffset)) {
            logError("Error writing file: failed to seek to offset %u\n", _offset + seekOffset);
            return false;
        }

        if (!_fileHandler->write(data, size)) {
            logError("Error writing file: failed to write data\n");
            return false;
        }

        return true;
    }

    virtual bool read(void* data, size_t size, size_t seekOffset = 0) const override {
        if (_fileHandler == nullptr) {
            logError("Error reading file: file handler is not set\n");
            return false;
        }

        if (!_fileHandler->seek(_offset + seekOffset)) {
            logError("Error reading file: failed to seek to offset %u\n", _offset + seekOffset);
            return false;
        }

        if (!_fileHandler->read(data, size)) {
            return false;
        }

        return true;
    }
};