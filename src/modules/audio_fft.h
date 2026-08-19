#pragma once

#include <stddef.h>
#include <stdint.h>

// Radix-2 FFT для вещественного сигнала (размер — степень двойки).
class AudioFft {
public:
    explicit AudioFft(uint16_t size);

    AudioFft(const AudioFft &) = delete;
    AudioFft &operator=(const AudioFft &) = delete;

    uint16_t size() const { return _size; }

    // real[] — вход длиной size(), imag[] заполняется нулями внутри.
    // После вызова real/imag содержат комплексный спектр.
    void forward(float *real, float *imag) const;

private:
    uint16_t _size;
    float *_cosTable;
    float *_sinTable;
    uint16_t *_bitRev;

    void initTables();
};
