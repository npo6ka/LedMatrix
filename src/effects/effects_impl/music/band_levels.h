#pragma once

#include "core/input/InputSnapshot.h"

#include <math.h>
#include <stdint.h>

// Общий анализ звука для музыкальных режимов: берёт нижние полосы FFT,
// сам находит шумовой пол и приводит громкость к диапазону 0..1, где
// примерно 0.55 — обычная громкость полосы, а удары уходят заметно выше.
class MusicBandLevels {
public:
    static constexpr uint8_t kBands = 5;

    void reset() {
        _warmupFrames = 0;
        for (uint8_t i = 0; i < kBands; ++i) {
            _smooth[i] = 0.0f;
            _noise[i] = 0.0f;
            _mean[i] = kMinMean;
            _value[i] = 0.0f;
        }
    }

    // false — рисовать нечего: микрофона нет, спектр ещё не посчитан
    // или идёт замер шумового пола после входа в режим.
    bool update(const AudioSnapshot &audio) {
        if (!audio.available || !audio.bands || audio.bandCount == 0) {
            return false;
        }

        float raw[kBands] = {};
        float rawSum = 0.0f;
        for (uint8_t i = 0; i < kBands; ++i) {
            raw[i] = sourceAverage(audio.bands, audio.bandCount, i);
            rawSum += raw[i];
        }

        // Первые кадры после старта спектр ещё не посчитан: если принять эти
        // нули за шумовой пол, он останется нулевым и всё уйдёт в потолок.
        if (rawSum <= 0.0f) {
            return false;
        }

        for (uint8_t i = 0; i < kBands; ++i) {
            updateSmooth(i, raw[i]);
        }

        if (_warmupFrames < kWarmupFrames) {
            for (uint8_t i = 0; i < kBands; ++i) {
                if (_warmupFrames == 0 || raw[i] < _noise[i]) {
                    _noise[i] = raw[i];
                }
            }
            ++_warmupFrames;
            return false;
        }

        for (uint8_t i = 0; i < kBands; ++i) {
            updateNoise(i, raw[i]);

            // Верх не ограничиваем: иначе адаптивный масштаб полосы не увидит
            // реальные пики и будет упираться в потолок.
            float scaled = (bandDb(i) - kFloorDb) / (kCeilDb - kFloorDb);
            if (scaled < 0.0f) scaled = 0.0f;
            _value[i] = normalize(i, scaled);
        }
        return true;
    }

    float value(uint8_t band) const {
        return band < kBands ? _value[band] : 0.0f;
    }

private:
    static constexpr uint16_t kWarmupFrames = 20;

    // Порог над шумовым полом и верх динамического диапазона в дБ.
    // Фоновый шум сам по себе гуляет на десяток дБ, поэтому порог высокий.
    static constexpr float kNoiseMargin = 1.0f;
    static constexpr float kFloorDb = 10.0f;
    static constexpr float kCeilDb = 34.0f;
    // Типичную громкость полосы отдаём примерно этим значением.
    static constexpr float kTargetLevel = 0.55f;
    static constexpr float kMinMean = 0.12f;

    float _smooth[kBands] = {};
    float _noise[kBands] = {};
    float _mean[kBands] = {};
    float _value[kBands] = {};
    uint16_t _warmupFrames = 0;

    static float clamp01(float value) {
        if (value < 0.0f) return 0.0f;
        if (value > 1.0f) return 1.0f;
        return value;
    }

    // Логарифмические FFT-полосы объединяем попарно, верхние не используем.
    static float sourceAverage(const float *bands, uint16_t count, uint8_t band) {
        uint8_t from = static_cast<uint8_t>(band * 2U);
        uint8_t to = static_cast<uint8_t>(from + 1U);
        if (from >= count) return 0.0f;
        if (to >= count) to = static_cast<uint8_t>(count - 1U);

        float sum = 0.0f;
        uint8_t samples = 0;
        for (uint8_t i = from; i <= to; ++i) {
            sum += bands[i];
            ++samples;
        }
        return samples > 0 ? sum / samples : 0.0f;
    }

    // Сглаживание с быстрой атакой и медленным спадом: одиночное окно FFT
    // слишком шумное, чтобы гнать его прямо на светодиоды.
    void updateSmooth(uint8_t band, float raw) {
        if (_warmupFrames == 0) {
            _smooth[band] = raw;
            return;
        }
        const float speed = raw > _smooth[band] ? 0.55f : 0.40f;
        _smooth[band] += (raw - _smooth[band]) * speed;
    }

    // Пол оценивается по несглаженным значениям: сглаженные почти не
    // проваливаются между ударами, и пол перестаёт опускаться.
    // Вниз идём быстро, вверх — только небольшим относительным приростом.
    void updateNoise(uint8_t band, float raw) {
        if (raw < _noise[band]) {
            // Геометрический спуск сходится к минимуму за пару секунд из
            // любого стартового значения, но не проваливается ниже сигнала.
            const float decayed = _noise[band] * 0.97f;
            _noise[band] = raw > decayed ? raw : decayed;
        } else {
            _noise[band] *= 1.0006f;
        }
        if (_noise[band] < 1.0f) _noise[band] = 1.0f;
    }

    float bandDb(uint8_t band) const {
        const float floorLevel = _noise[band] * kNoiseMargin;
        if (floorLevel <= 0.0f) return 0.0f;
        const float snr = _smooth[band] / floorLevel;
        return snr > 1.0f ? 20.0f * log10f(snr) : 0.0f;
    }

    // Полоса нормируется своим средним уровнем: по максимуму картинка
    // всё время прижата к потолку, а по среднему видно отклонения.
    float normalize(uint8_t band, float value) {
        const float speed = value > _mean[band] ? 0.03f : 0.01f;
        _mean[band] += (value - _mean[band]) * speed;
        if (_mean[band] < kMinMean) _mean[band] = kMinMean;
        return clamp01(value * kTargetLevel / _mean[band]);
    }
};
