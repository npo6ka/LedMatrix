#pragma once

#include <stdint.h>

struct AudioSnapshot {
    bool available = false;
    const float *bands = nullptr;
    uint16_t bandCount = 0;
    float rms = 0.0f;
    uint16_t dominantBand = 0;

    void clear() {
        available = false;
        bands = nullptr;
        bandCount = 0;
        rms = 0.0f;
        dominantBand = 0;
    }
};

struct InputSnapshot {
    uint32_t frameMs = 0;
    AudioSnapshot audio;

    void clear() {
        frameMs = 0;
        audio.clear();
    }
};
