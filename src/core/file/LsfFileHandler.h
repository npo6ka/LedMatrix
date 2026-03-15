#pragma once

#include "IFileHandler.h"
#include "libs/debug_lib.h"
#include <LittleFS.h>

#if defined(ESP32)
using namespace fs;
#endif

class LsfFileHandler : public IFileHandler {
    File _file;

    LsfFileHandler(const LsfFileHandler&) = delete;
    LsfFileHandler& operator=(const LsfFileHandler&) = delete;
    LsfFileHandler(LsfFileHandler&&) = delete;
    LsfFileHandler& operator=(LsfFileHandler&&) = delete;
public:
    LsfFileHandler() {
        static bool isMounted = false;
        if (!isMounted) {
#if defined(ESP8266)
            isMounted = LittleFS.begin();
#elif defined(ESP32)
            // Сначала без форматирования, при неудаче — с форматированием (пустой/битый раздел).
            isMounted = LittleFS.begin(false) || LittleFS.begin(true);
#else
#   error "LsfFileHandler: unsupported platform"
#endif
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
            logError("Error opening file: filename is too long\n");
            return;
        }
        if (_file) {
            logError("file is already open, closing it first\n");
            close();
        }

        char buffer[34];
        buffer[0] = '/';
        strncpy(buffer + 1, path, 32);
        buffer[33] = '\0';

        _file = LittleFS.open(buffer, "r+");
        if (!_file) {
            _file = LittleFS.open(buffer, "w+");
            if (!_file) {
                logError("Error opening file: %s, failed to create file\n", path);
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
            logError("Error writing file: file is not open\n");
            return false;
        }
        if (seekOffset != 0 && !_file.seek(seekOffset, SeekSet)) {
            logError("Error writing file: failed to seek to offset %zu\n", seekOffset);
            return false;
        }
        size_t written = _file.write(reinterpret_cast<const uint8_t*>(data), size);
        if (written != size) {
            logError("Error writing file: failed to write data\n");
            return false;
        }
        _file.flush();
        return true;
    }

    virtual bool read(void* data, size_t size, size_t seekOffset = 0) const override {
        if (!_file) {
            logError("Error reading file: file is not open\n");
            return false;
        }
        if (seekOffset != 0 && !const_cast<File&>(_file).seek(seekOffset, SeekSet)) {
            logError("Error reading file: failed to seek to offset %zu\n", seekOffset);
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
            logError("Error seeking file: file is not open\n");
            return false;
        }
        return _file.seek(position, SeekSet);
    }

    virtual size_t size() const override {
        if (!_file) {
            logError("Error getting file size: file is not open\n");
            return 0;
        }
        return _file.size();
    }
};
