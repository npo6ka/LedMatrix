#pragma once

#include "effects/effect.h"
#include "effects/effects_impl/music/band_levels.h"

class MusicSpectrum : public Effect {
    static constexpr uint8_t kDisplayBands = MusicBandLevels::kBands;

    // Спад столбца за кадр: полное гашение примерно за полторы секунды.
    static constexpr float kLevelDecay = 0.02f;
    // Тихо — холодный синий, громко — красный.
    static constexpr uint8_t kQuietHue = 160;
    // Доля полосы у каждого края, на которой она перетекает в соседнюю.
    static constexpr float kBlendZone = 0.10f;

    MusicBandLevels _analyzer;
    float _level[kDisplayBands] = {};

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
        _analyzer.reset();
        for (uint8_t i = 0; i < kDisplayBands; ++i) {
            _level[i] = 0.0f;
        }
    }

    void on_update(const InputSnapshot &input) override {
        if (!_analyzer.update(input.audio)) {
            LedMatrix.clear();
            return;
        }

        for (uint8_t i = 0; i < kDisplayBands; ++i) {
            const float target = _analyzer.value(i);
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
