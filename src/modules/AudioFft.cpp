#include "AudioFft.h"

#include <Arduino.h>
#include <math.h>
#include <stdlib.h>

namespace {

uint16_t reverseBits(uint16_t value, uint16_t bits) {
    uint16_t out = 0;
    for (uint16_t i = 0; i < bits; ++i) {
        out = static_cast<uint16_t>((out << 1) | (value & 1U));
        value = static_cast<uint16_t>(value >> 1);
    }
    return out;
}

uint16_t log2u(uint16_t value) {
    uint16_t bits = 0;
    while (value > 1U) {
        value = static_cast<uint16_t>(value >> 1);
        ++bits;
    }
    return bits;
}

bool isPowerOfTwo(uint16_t value) {
    return value > 0U && (value & (value - 1U)) == 0U;
}

} // namespace

AudioFft::AudioFft(uint16_t size) : _size(size), _cosTable(nullptr), _sinTable(nullptr), _bitRev(nullptr) {
    initTables();
}

void AudioFft::initTables() {
    if (!isPowerOfTwo(_size)) {
        _size = 256;
    }

    const uint16_t half = static_cast<uint16_t>(_size / 2U);
    const uint16_t bits = log2u(_size);

    _cosTable = static_cast<float *>(malloc(sizeof(float) * half));
    _sinTable = static_cast<float *>(malloc(sizeof(float) * half));
    _bitRev = static_cast<uint16_t *>(malloc(sizeof(uint16_t) * _size));

    if (!_cosTable || !_sinTable || !_bitRev) {
        return;
    }

    for (uint16_t i = 0; i < half; ++i) {
        const float angle = -2.0f * static_cast<float>(M_PI) * static_cast<float>(i) / static_cast<float>(_size);
        _cosTable[i] = cosf(angle);
        _sinTable[i] = sinf(angle);
    }

    for (uint16_t i = 0; i < _size; ++i) {
        _bitRev[i] = reverseBits(i, bits);
    }
}

void AudioFft::forward(float *real, float *imag) const {
    if (!real || !imag || !_cosTable || !_sinTable || !_bitRev) {
        return;
    }

    for (uint16_t i = 0; i < _size; ++i) {
        const uint16_t j = _bitRev[i];
        if (j > i) {
            const float tr = real[i];
            real[i] = real[j];
            real[j] = tr;
            const float ti = imag[i];
            imag[i] = imag[j];
            imag[j] = ti;
        }
    }

    for (uint16_t len = 2; len <= _size; len = static_cast<uint16_t>(len << 1)) {
        const uint16_t halfLen = static_cast<uint16_t>(len / 2U);
        const uint16_t step = static_cast<uint16_t>(_size / len);

        for (uint16_t i = 0; i < _size; i = static_cast<uint16_t>(i + len)) {
            for (uint16_t j = 0; j < halfLen; ++j) {
                const uint16_t twIdx = static_cast<uint16_t>(j * step);
                const float wr = _cosTable[twIdx];
                const float wi = _sinTable[twIdx];

                const uint16_t even = static_cast<uint16_t>(i + j);
                const uint16_t odd = static_cast<uint16_t>(even + halfLen);

                const float tr = wr * real[odd] - wi * imag[odd];
                const float ti = wr * imag[odd] + wi * real[odd];

                real[odd] = real[even] - tr;
                imag[odd] = imag[even] - ti;
                real[even] += tr;
                imag[even] += ti;
            }
        }
    }
}
