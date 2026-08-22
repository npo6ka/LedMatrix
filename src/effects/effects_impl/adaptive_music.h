#pragma once

#include "effects/effect.h"

#include <math.h>

class AdaptiveMusic : public Effect {
    static constexpr uint8_t kDisplayBands = 5;
    static constexpr uint16_t kWarmupFrames = 20;

    // Порог над шумовым полом и верх динамического диапазона в дБ.
    // Фоновый шум сам по себе гуляет на десяток дБ, поэтому порог высокий.
    static constexpr float kNoiseMargin = 1.0f;
    static constexpr float kFloorDb = 10.0f;
    static constexpr float kCeilDb = 34.0f;
    // Типичный уровень полосы показываем примерно на этой высоте, удары уходят выше.
    static constexpr float kTargetLevel = 0.55f;
    static constexpr float kMinMean = 0.12f;
    // Спад столбца за кадр: полное гашение примерно за полторы секунды.
    static constexpr float kLevelDecay = 0.02f;
    // Тихо — холодный синий, громко — красный.
    static constexpr uint8_t kQuietHue = 160;
    // Доля полосы у каждого края, на которой она перетекает в соседнюю.
    static constexpr float kBlendZone = 0.10f;

    float _smooth[kDisplayBands] = {};
    float _noise[kDisplayBands] = {};
    float _mean[kDisplayBands] = {};
    float _level[kDisplayBands] = {};
    uint16_t _warmupFrames = 0;

    static float clamp01(float value) {
        if (value < 0.0f) return 0.0f;
        if (value > 1.0f) return 1.0f;
        return value;
    }

    static void sourceRange(uint8_t band, uint8_t &from, uint8_t &to) {
        // Логарифмические FFT-полосы объединяем попарно, верхние не используем.
        from = static_cast<uint8_t>(band * 2U);
        to = static_cast<uint8_t>(from + 1U);
    }

    static float sourceAverage(
            const float *bands,
            uint16_t count,
            uint8_t from,
            uint8_t to) {
        if (!bands || count == 0 || from >= count) return 0.0f;
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

    // Уровень в конкретной точке ленты. Центральные 80% полосы показывают
    // её собственный уровень, а у краёв полосы заходят друг на друга: на
    // самом стыке получается ровно половина от каждой из двух соседних.
    float levelAt(index_t x, index_t width) const {
        const float pos = (static_cast<float>(x) + 0.5f)
            * static_cast<float>(kDisplayBands) / static_cast<float>(width);

        int16_t band = static_cast<int16_t>(pos);
        if (band < 0) band = 0;
        if (band >= static_cast<int16_t>(kDisplayBands)) {
            band = static_cast<int16_t>(kDisplayBands) - 1;
        }
        const float inBand = pos - static_cast<float>(band);

        int16_t neighbour = -1;
        float mix = 0.0f;
        if (inBand < kBlendZone) {
            neighbour = static_cast<int16_t>(band - 1);
            mix = 1.0f - inBand / kBlendZone;
        } else if (inBand > 1.0f - kBlendZone) {
            neighbour = static_cast<int16_t>(band + 1);
            mix = (inBand - (1.0f - kBlendZone)) / kBlendZone;
        }

        if (neighbour < 0 || neighbour >= static_cast<int16_t>(kDisplayBands)) {
            return _level[band];
        }

        // S-образная кривая вместо линейной: иначе граница зоны смешивания
        // сама становится видимым изломом яркости.
        const float weight = mix * mix * (3.0f - 2.0f * mix) * 0.5f;
        return _level[band] * (1.0f - weight) + _level[neighbour] * weight;
    }

    void paint() {
        const index_t width = LedMatrix.width();
        const index_t height = LedMatrix.height();
        LedMatrix.clear();
        if (width == 0 || height == 0) return;

        for (index_t x = 0; x < width; ++x) {
            const float level = levelAt(x, width);
            if (level <= 0.02f) continue;

            const uint8_t hue = static_cast<uint8_t>(
                static_cast<float>(kQuietHue) * (1.0f - level) + 0.5f);
            // На ленте высоты нет, поэтому громкость передаём ещё и яркостью.
            // Квадрат нужен, чтобы между ударами сегмент уходил в темноту,
            // а не тлел: глаз воспринимает яркость нелинейно.
            const uint8_t value = static_cast<uint8_t>(level * level * 255.0f + 0.5f);

            index_t barHeight = static_cast<index_t>(
                level * static_cast<float>(height) + 0.5f);
            if (barHeight == 0) barHeight = 1;

            for (index_t y = 0; y < barHeight && y < height; ++y) {
                const index_t row = static_cast<index_t>(height - 1U - y);
                LedMatrix.at(x, row) = CHSV(hue, 255, value);
            }
        }
    }

public:
    static constexpr InputCapability kRequired = InputCapability::Audio;

    void on_init() override {
        set_fps(30);
        _warmupFrames = 0;
        for (uint8_t i = 0; i < kDisplayBands; ++i) {
            _smooth[i] = 0.0f;
            _noise[i] = 0.0f;
            _mean[i] = kMinMean;
            _level[i] = 0.0f;
        }
    }

    void on_update(const InputSnapshot &input) override {
        if (!input.audio.available || !input.audio.bands || input.audio.bandCount == 0) {
            LedMatrix.clear();
            return;
        }

        float raw[kDisplayBands] = {};
        float rawSum = 0.0f;
        for (uint8_t i = 0; i < kDisplayBands; ++i) {
            uint8_t from = 0;
            uint8_t to = 0;
            sourceRange(i, from, to);
            raw[i] = sourceAverage(input.audio.bands, input.audio.bandCount, from, to);
            rawSum += raw[i];
        }

        // Первые кадры после старта спектр ещё не посчитан: если принять эти
        // нули за шумовой пол, он останется нулевым и всё уйдёт в потолок.
        if (rawSum <= 0.0f) {
            LedMatrix.clear();
            return;
        }

        for (uint8_t i = 0; i < kDisplayBands; ++i) {
            updateSmooth(i, raw[i]);
        }

        if (_warmupFrames < kWarmupFrames) {
            for (uint8_t i = 0; i < kDisplayBands; ++i) {
                if (_warmupFrames == 0 || raw[i] < _noise[i]) {
                    _noise[i] = raw[i];
                }
            }
            ++_warmupFrames;
            LedMatrix.clear();
            return;
        }

        for (uint8_t i = 0; i < kDisplayBands; ++i) {
            updateNoise(i, raw[i]);

            // Верх не ограничиваем: иначе адаптивный масштаб полосы не увидит
            // реальные пики и будет упираться в потолок.
            float scaled = (bandDb(i) - kFloorDb) / (kCeilDb - kFloorDb);
            if (scaled < 0.0f) scaled = 0.0f;
            const float target = normalize(i, scaled);

            // Пик подхватываем сразу, дальше столбец плавно оседает,
            // пока в этой полосе не придёт следующий удар.
            if (target > _level[i]) {
                _level[i] = target;
            } else {
                _level[i] -= kLevelDecay;
                if (_level[i] < 0.0f) _level[i] = 0.0f;
            }
        }

        paint();
    }

    void on_update() override {}
};
