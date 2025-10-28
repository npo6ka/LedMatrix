#pragma once

#include "IFileHandler.h"
#include "libs/debug_lib.h"

#include "LittleFS.h"

class LsfFileHandler : public IFileHandler {
    File _file;

    LsfFileHandler(const LsfFileHandler&) = delete;
    LsfFileHandler& operator= (const LsfFileHandler&) = delete;
    LsfFileHandler(LsfFileHandler&& other) = delete;
    LsfFileHandler& operator= (LsfFileHandler&& other) = delete;
public:
    LsfFileHandler() {
        static bool isMounted = false;
        if (!isMounted && LittleFS.begin()) {
            isMounted = true;
        }
    }

    LsfFileHandler(const char* path) : LsfFileHandler() {
        open(path);
    }

    virtual ~LsfFileHandler() {
        close();
    }

    virtual void open(const char* path) override {
        if (strlen(path) > 32) {
            logError("filename is too long");
            return;
        }
        if (_file) {
            logInfo("file is already open, closing it first");
            return;
        }

        char buffer[33];
        strncpy(buffer + 1, path, 32);
        buffer[0] = '/';

        _file = LittleFS.open(buffer, "r+");
        if (!_file) {
            _file = LittleFS.open(buffer, "w+");
            if (!_file) {
                logError("failed to create file");
            }
        }
    }

    virtual void close() override {
        if (_file) {
            _file.close();
        }
    }

    virtual bool write(const void* data, size_t size, size_t seekOffset = 0) override {
        if (!_file) {
            logError("file is not open for writing");
            return false;
        }

        size_t written = _file.write(reinterpret_cast<const uint8_t*>(data), size);
        if (written != size) {
            logError("failed to write all data");
            return false;
        }

        return true;
    }

    virtual bool read(void* data, size_t size, size_t seekOffset = 0) const override {
        if (!_file) {
            logError("file is not open for reading");
            return false;
        }

        size_t readBytes = const_cast<File&>(_file).read(reinterpret_cast<uint8_t*>(data), size);
        if (readBytes != size) {
            return false;
        }

        return true;
    }

    virtual bool seek(size_t position) override {
        if (!_file) {
            logError("file is not open for seeking");
            return false;
        }
        if (_file.position() == position) {
            return true;
        }

        return _file.seek(position, SeekSet);
    }

    virtual size_t size() const override {
        if (!_file) {
            logError("file is not open to get size");
            return 0;
        }

        return _file.size();
    }
};