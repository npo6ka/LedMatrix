#pragma once

#include "IFileHandler.h"
#include <LITTLEFS.h>

class Lsf32FileHandler : public IFileHandler {
/*    FILE *_file = nullptr;

    Lsf32FileHandler(const Lsf32FileHandler&) = delete;
    Lsf32FileHandler& operator= (const Lsf32FileHandler&) = delete;
public:
    Lsf32FileHandler() = default;
    virtual ~Lsf32FileHandler() {
        close();
    };

    Lsf32FileHandler(Lsf32FileHandler&& other) {
        _file = other._file;
        other._file = nullptr;
    }

    Lsf32FileHandler& operator= (Lsf32FileHandler&& other) {
        _file = other._file;
        other._file = nullptr;
        return *this;
    }

    virtual void open(const char* path) override {
        if (strlen(path) > 32) {
            logError("Error opening file: filename is too long\n");
            return;
        }
        logError("called open with path: %s\n", path);
        if ((_file = fopen(path, "r+")) == nullptr) {
            logError("file not opened\n");
            if ((_file = fopen(path, "w+")) == nullptr) {
                logError("Error opening file: %s, failed to create file\n", path);
            }
        } else {
            logError("file opened successfully\n");
        }
    }

    virtual void close() override {
        if (_file != nullptr) {
            fclose(_file);
            _file = nullptr;
        }
    }

    virtual bool write(const void* data, size_t size) override {
        if (_file == nullptr) {
            logError("Error writing file: file is not open\n");
            return false;
        }

        if (fwrite(data, size, 1, _file) != 1) {
            logError("Error writing file: failed to write data\n");
            return false;
        }

        return true;
    }

    virtual bool read(void* data, size_t size) const override {
        if (!_file) {
            logError("Error reading file: file is not open\n");
            return false;
        }

        if (fread(data, size, 1, _file) != 1) {
            return false;
        }

        return true;
    }

    virtual bool seek(size_t position) override {
        if (_file == nullptr) {
            logError("Error seeking file: file is not open\n");
            return false;
        }

        if (fseek(_file, position, SEEK_SET) != 0) {
            logError("Error seeking file: failed to seek to position %zu\n", position);
            return false;
        }

        return true;
    }

    virtual size_t size() const override {
        if (_file == nullptr) {
            logError("Error getting file size: file is not open\n");
            return 0;
        }

        long currentPos = ftell(_file);
        fseek(_file, 0, SEEK_END);
        long fileSize = ftell(_file);
        fseek(_file, currentPos, SEEK_SET);

        return static_cast<size_t>(fileSize);
    }*/
};