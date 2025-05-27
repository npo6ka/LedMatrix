#pragma once

// #include "LittleFS.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define out printf
#define MAX_FILE_SIZE 65536
#define STEP

class Memory {
    FILE *fp = nullptr;

    // size_t size() {
    //     fseek(fp, 0L, SEEK_END);
    //     return ftell(fp);
    // }

    void open_file(const char *filename) {
        fp = fopen(filename, "r+");
        if (!fp) {
            if (fp = fopen(filename, "w")) {
                fclose(fp);
            }
            fp = fopen(filename, "r+");
        }
        fseek(fp, MAX_FILE_SIZE - 1, SEEK_SET);
        fputc('\0', fp);
    }

    // template<class T>
    // void validate_memory(uint32_t addr, T &val) {
    //     size_t size = size();
    //     if (size < addr + sizeof(val)) {

    //     }
    // }

    Memory(const Memory&) = delete;
    Memory& operator= (const Memory&) = delete;

public:
    Memory() {}
    Memory(const char* filename) {
        open_file(filename/*"mods.dat"*/);
    }

    ~Memory() {
        fclose(fp);
    }

    Memory(Memory&& other){
        fp = other.fp;
        other.fp = nullptr;
    };
    Memory& operator= (Memory&& other) {
        fp = other.fp;
        other.fp = nullptr;
        return *this;
    }

    template<class T>
    bool read(uint32_t addr, T &val) {
        if (!fp) {
            out("Error reading ROM: memory file is NULL\n");
            return false;
        }
        if (addr + sizeof(T) < MAX_FILE_SIZE) {
            fseek(fp, addr, SEEK_SET);
            return fread(&val, sizeof(T), 1, fp) == 1;
        } else {
            out("Error reading ROM: position out of memory\n");
        }

        return false;
    }

    template<class T>
    bool write(uint32_t addr, const T val) {
        if (!fp) {
            out("Error writing ROM: memory file is NULL\n");
            return false;
        }
        if (addr + sizeof(T) < MAX_FILE_SIZE) {
            fseek(fp, addr, SEEK_SET);
            // printf("write to 0x%x, res: %d\n", addr, fwrite(&val, sizeof(T), 1, fp) == 1);
            fwrite(&val, sizeof(T), 1, fp);
            return true;
        } else {
            out("Error writing ROM: position out of memory\n");
        }

        return false;
    }

    void clear() {
        if (!fp) {
            out("Error writing ROM: memory file is NULL\n");
            return;
        }

        fseek(fp, 0, SEEK_SET);
        uint8_t val = 0;
        for (uint32_t i = 0; i < MAX_FILE_SIZE; ++i) {
            fwrite(&val, sizeof(val), 1, fp);
        }
    }
};