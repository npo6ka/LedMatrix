/*#pragma once

#include "LittleFS.h"
#include <stdlib.h>
#include <stdint.h>

#define MAX_FILE_SIZE 65536
#define STEP

class FileHandler {
    FILE *_fp = nullptr;

    FileHandler(const FileHandler&) = delete;
    FileHandler& operator= (const FileHandler&) = delete;

public:
    FileHandler() = default;

    ~FileHandler() = default;

    FileHandler(FileHandler&& other){
        _fp = other._fp;
        other._fp = nullptr;
    };
    FileHandler& operator= (FileHandler&& other) {
        _fp = other._fp;
        other._fp = nullptr;
        return *this;
    }

    void openFile(const char *filename) {
        if (strlen(filename) > 32) {
            out("Error opening file: filename is too long\n");
            return;
        }
        if (!_fp) {
            if ((_fp = fopen(filename, "w")) != nullptr) {
                fclose(_fp);
            }
            _fp = fopen(filename, "r+");
        }
        if (fseek(_fp, MAX_FILE_SIZE - 1, SEEK_SET) != 0) {
            out("Error opening file: failed to seek to end of file\n");
            _fp = nullptr;
            return;
        }
        fputc('\0', _fp);
    }

    template<class T>
    bool read(uint32_t addr, T &val) {
        if (!_fp) {
            out("Error reading ROM: memory file is NULL\n");
            return false;
        }
        if (addr + sizeof(T) < MAX_FILE_SIZE) {
            fseek(_fp, addr, SEEK_SET);
            return fread(&val, sizeof(T), 1, _fp) == 1;
        } else {
            out("Error reading ROM: position out of memory\n");
        }

        return false;
    }

    template<class T>
    bool write(uint32_t addr, const T val) {
        if (!_fp) {
            out("Error writing ROM: memory file is NULL\n");
            return false;
        }
        if (addr + sizeof(T) < MAX_FILE_SIZE) {
            fseek(_fp, addr, SEEK_SET);
            // printf("write to 0x%x, res: %d\n", addr, fwrite(&val, sizeof(T), 1, fp) == 1);
            fwrite(&val, sizeof(T), 1, _fp);
            return true;
        } else {
            out("Error writing ROM: position out of memory\n");
        }

        return false;
    }

    void clear() {
        if (!_fp) {
            out("Error writing ROM: memory file is NULL\n");
            return;
        }

        fseek(_fp, 0, SEEK_SET);
        uint8_t val = 0;
        for (uint32_t i = 0; i < MAX_FILE_SIZE; ++i) {
            fwrite(&val, sizeof(val), 1, _fp);
        }
    }
};*/