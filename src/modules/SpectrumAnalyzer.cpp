#include "SpectrumAnalyzer.h"

#include <Arduino.h>
#include <math.h>
#include <stdlib.h>

namespace {

bool isPowerOfTwo(uint16_t value) {
    return value > 0U && (value & (value - 1U)) == 0U;
}

uint16_t clampBands(uint16_t bands) {
    if (bands < 1U) return 1U;
    if (bands > SpectrumAnalyzer::kMaxBands) return SpectrumAnalyzer::kMaxBands;
    return bands;
}

uint16_t clampFftSize(uint16_t size) {
    if (!isPowerOfTwo(size) || size > SpectrumAnalyzer::kMaxFftSize) {
        return 256;
    }
    return size;
}

} // namespace

SpectrumAnalyzer::SpectrumAnalyzer(uint32_t sampleRate, uint16_t fftSize, uint16_t bandCount)
        : _fft(clampFftSize(fftSize)),
          _sampleRate(sampleRate),
          _fftSize(clampFftSize(fftSize)),
          _bandCount(clampBands(bandCount)),
          _writeIndex(0),
          _ready(false),
          _window(nullptr),
          _timeReal(nullptr),
          _timeImag(nullptr),
          _magnitudes(nullptr),
          _bandBands(nullptr),
          _bandStarts(nullptr) {
    _window = static_cast<float *>(calloc(_fftSize, sizeof(float)));
    _timeReal = static_cast<float *>(calloc(_fftSize, sizeof(float)));
    _timeImag = static_cast<float *>(calloc(_fftSize, sizeof(float)));
    _magnitudes = static_cast<float *>(calloc(_fftSize / 2U, sizeof(float)));
    _bandBands = static_cast<float *>(calloc(_bandCount, sizeof(float)));
    _bandStarts = static_cast<uint16_t *>(calloc(_bandCount + 1U, sizeof(uint16_t)));

    if (_window && _fftSize > 1U) {
        for (uint16_t i = 0; i < _fftSize; ++i) {
            _window[i] = 0.5f * (1.0f - cosf(2.0f * static_cast<float>(M_PI) * static_cast<float>(i)
                                             / static_cast<float>(_fftSize - 1U)));
        }
    }

    buildBands();
    reset();
}

void SpectrumAnalyzer::reset() {
    _writeIndex = 0;
    _ready = false;
    if (_timeReal) {
        for (uint16_t i = 0; i < _fftSize; ++i) {
            _timeReal[i] = 0.0f;
            _timeImag[i] = 0.0f;
        }
    }
    if (_magnitudes) {
        for (uint16_t i = 0; i < _fftSize / 2U; ++i) {
            _magnitudes[i] = 0.0f;
        }
    }
    if (_bandBands) {
        for (uint16_t i = 0; i < _bandCount; ++i) {
            _bandBands[i] = 0.0f;
        }
    }
}

bool SpectrumAnalyzer::pushSample(float sample) {
    if (!_timeReal) {
        return false;
    }

    _timeReal[_writeIndex] = sample;
    _timeImag[_writeIndex] = 0.0f;
    ++_writeIndex;

    if (_writeIndex < _fftSize) {
        return false;
    }

    _writeIndex = 0;
    compute();
    _ready = true;
    return true;
}

bool SpectrumAnalyzer::pushSamples(const int32_t *samples, size_t count, int32_t dc) {
    if (!samples || count == 0U) {
        return false;
    }

    bool ready = false;
    for (size_t i = 0; i < count; ++i) {
        const float sample = static_cast<float>(samples[i] - dc);
        if (pushSample(sample)) {
            ready = true;
        }
    }
    return ready;
}

void SpectrumAnalyzer::applyWindow() {
    if (!_window || !_timeReal) {
        return;
    }
    for (uint16_t i = 0; i < _fftSize; ++i) {
        _timeReal[i] *= _window[i];
        _timeImag[i] = 0.0f;
    }
}

void SpectrumAnalyzer::compute() {
    if (!_timeReal || !_timeImag || !_magnitudes || !_bandBands) {
        return;
    }

    applyWindow();
    _fft.forward(_timeReal, _timeImag);

    const uint16_t half = static_cast<uint16_t>(_fftSize / 2U);
    _magnitudes[0] = fabsf(_timeReal[0]);

    for (uint16_t i = 1; i < half; ++i) {
        const float re = _timeReal[i];
        const float im = _timeImag[i];
        _magnitudes[i] = sqrtf(re * re + im * im);
    }

    for (uint16_t b = 0; b < _bandCount; ++b) {
        uint16_t start = _bandStarts[b];
        uint16_t end = _bandStarts[b + 1U];
        if (end <= start) {
            end = start + 1U;
        }
        if (end > half) {
            end = half;
        }
        float sum = 0.0f;
        for (uint16_t i = start; i < end && i < half; ++i) {
            sum += _magnitudes[i];
        }
        const uint16_t count = end - start;
        _bandBands[b] = sum / static_cast<float>(count);
    }
}

void SpectrumAnalyzer::buildBands() {
    if (!_bandStarts || _bandCount == 0U) {
        return;
    }

    const float minHz = 80.0f;
    const float maxHz = static_cast<float>(_sampleRate) * 0.45f;
    const float binHz = static_cast<float>(_sampleRate) / static_cast<float>(_fftSize);
    const uint16_t half = static_cast<uint16_t>(_fftSize / 2U);

    for (uint16_t b = 0; b <= _bandCount; ++b) {
        const float t = static_cast<float>(b) / static_cast<float>(_bandCount);
        const float hz = minHz * powf(maxHz / minHz, t);

        uint16_t bin = static_cast<uint16_t>(hz / binHz);
        if (bin < 1U) {
            bin = 1U;
        }
        if (bin > half) {
            bin = half;
        }
        _bandStarts[b] = bin;
    }

    // Каждая полоса должна включать хотя бы один бин
    for (uint16_t b = 1; b <= _bandCount; ++b) {
        if (_bandStarts[b] <= _bandStarts[b - 1U]) {
            _bandStarts[b] = _bandStarts[b - 1U] + 1U;
        }
        if (_bandStarts[b] > half) {
            _bandStarts[b] = half;
        }
    }

    for (int16_t b = static_cast<int16_t>(_bandCount) - 1; b >= 0; --b) {
        const uint16_t idx = static_cast<uint16_t>(b);
        if (_bandStarts[idx + 1U] <= _bandStarts[idx]) {
            _bandStarts[idx] = (_bandStarts[idx + 1U] > 1U) ? _bandStarts[idx + 1U] - 1U : 1U;
        }
    }

    _bandStarts[_bandCount] = half;
}

float SpectrumAnalyzer::binToHz(uint16_t bin) const {
    return static_cast<float>(bin) * static_cast<float>(_sampleRate) / static_cast<float>(_fftSize);
}

float SpectrumAnalyzer::bandToHz(uint16_t band) const {
    if (band >= _bandCount) {
        band = static_cast<uint16_t>(_bandCount - 1U);
    }
    const float minHz = 80.0f;
    const float maxHz = static_cast<float>(_sampleRate) * 0.45f;
    const float t = (static_cast<float>(band) + 0.5f) / static_cast<float>(_bandCount);
    return minHz * powf(maxHz / minHz, t);
}

uint16_t SpectrumAnalyzer::dominantBand() const {
    if (!_bandBands || _bandCount == 0U) {
        return 0;
    }

    uint16_t best = 0;
    float peak = _bandBands[0];
    for (uint16_t i = 1; i < _bandCount; ++i) {
        if (_bandBands[i] > peak) {
            peak = _bandBands[i];
            best = i;
        }
    }
    return best;
}
