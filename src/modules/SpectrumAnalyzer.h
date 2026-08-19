#pragma once

#include "AudioFft.h"

#include <stddef.h>
#include <stdint.h>

// Накопление окна PCM -> FFT -> амплитуды частот -> логарифмические полосы.
class SpectrumAnalyzer {
public:
    static constexpr uint16_t kMaxFftSize = 512;
    static constexpr uint16_t kMaxBands = 32;

    SpectrumAnalyzer(uint32_t sampleRate, uint16_t fftSize, uint16_t bandCount);

    SpectrumAnalyzer(const SpectrumAnalyzer &) = delete;
    SpectrumAnalyzer &operator=(const SpectrumAnalyzer &) = delete;

    void reset();

    // Добавить один сэмпл (после удаления DC). true — окно заполнено и спектр пересчитан.
    bool pushSample(float sample);

    // Добавить блок сэмплов int32 (I2S >> 8). dc — среднее окна для центрирования.
    bool pushSamples(const int32_t *samples, size_t count, int32_t dc = 0);

    bool isReady() const { return _ready; }
    void clearReady() { _ready = false; }

    uint16_t fftSize() const { return _fftSize; }
    uint16_t binCount() const { return static_cast<uint16_t>(_fftSize / 2U); }
    uint16_t bandCount() const { return _bandCount; }

    const float *bins() const { return _magnitudes; }
    const float *bands() const { return _bandBands; }

    float binToHz(uint16_t bin) const;
    float bandToHz(uint16_t band) const;
    uint16_t dominantBand() const;

private:
    AudioFft _fft;
    uint32_t _sampleRate;
    uint16_t _fftSize;
    uint16_t _bandCount;
    uint16_t _writeIndex;
    bool _ready;

    float *_window;
    float *_timeReal;
    float *_timeImag;
    float *_magnitudes;
    float *_bandBands;
    uint16_t *_bandStarts;

    void compute();
    void buildBands();
    void applyWindow();
};
