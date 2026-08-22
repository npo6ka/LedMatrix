#pragma once

#include "effects/effect.h"
#include "effects/effects_impl/music/band_levels.h"

class MusicSlider : public Effect {
    static constexpr uint8_t kBands = MusicBandLevels::kBands;
    static constexpr uint8_t kFps = 60;

    // Вверх слайдер прыгает мгновенно, вниз оседает примерно за полсекунды.
    static constexpr float kDecay = 0.03f;
    // Рабочий участок громкости, который растягиваем на всю длину ленты:
    // сама по себе средняя энергия держится около 0.55 и почти не доходит
    // до краёв диапазона, поэтому слайдер стоял бы полным.
    static constexpr float kQuietEnergy = 0.35f;
    static constexpr float kLoudEnergy = 0.85f;
    // Ниже этого уровня считаем, что звука нет, и гасим ленту совсем.
    static constexpr float kMinLevel = 0.03f;
    // Лёгкое послесвечение за откатившимся краем.
    static constexpr uint8_t kTrailFade = 60;
    // Доля пути до нужного оттенка за кадр: смена преобладающей полосы
    // иначе перекрашивает ленту скачком.
    static constexpr float kHueSpeed = 0.04f;

    MusicBandLevels _analyzer;
    float _level = 0.0f;
    float _hue = 0.0f;

    // Оттенок ведём к цели по короткой стороне цветового круга, иначе
    // переход с красного на фиолетовый пойдёт через всю радугу.
    void updateHue(uint8_t target) {
        float diff = static_cast<float>(target) - _hue;
        if (diff > 128.0f) diff -= 256.0f;
        else if (diff < -128.0f) diff += 256.0f;

        _hue += diff * kHueSpeed;
        if (_hue < 0.0f) _hue += 256.0f;
        if (_hue >= 256.0f) _hue -= 256.0f;
    }

    index_t runLength() const {
        const index_t width = LedMatrix.width();
        const index_t height = LedMatrix.height();
        return (width <= 1) ? height : width;
    }

    // Слайдер занимает всю поперечину матрицы: на ленте это просто пиксель,
    // на матрице — столбец целиком.
    void drawAcross(index_t along, uint8_t amount) {
        if (amount == 0 || along >= runLength()) return;

        const CRGB color = CHSV(static_cast<uint8_t>(_hue), 255, 255);
        const index_t width = LedMatrix.width();
        const index_t height = LedMatrix.height();

        if (width <= 1) {
            for (index_t x = 0; x < width; ++x) {
                CRGB &pixel = LedMatrix.at(x, along);
                pixel = blend(pixel, color, amount);
            }
            return;
        }
        for (index_t y = 0; y < height; ++y) {
            CRGB &pixel = LedMatrix.at(along, y);
            pixel = blend(pixel, color, amount);
        }
    }

    void paint() {
        const index_t length = runLength();
        if (length == 0 || _level <= kMinLevel) return;

        const float center = static_cast<float>(length) * 0.5f;
        const float half = _level * center;
        const float left = center - half;
        const float right = center + half;

        for (index_t i = 0; i < length; ++i) {
            const float pixelLeft = static_cast<float>(i);
            const float pixelRight = pixelLeft + 1.0f;

            // Считаем, какую часть пикселя накрывает слайдер: так края
            // двигаются плавно, а не прыгают целыми диодами.
            const float from = left > pixelLeft ? left : pixelLeft;
            const float to = right < pixelRight ? right : pixelRight;
            float covered = to - from;
            if (covered <= 0.0f) continue;
            if (covered > 1.0f) covered = 1.0f;

            drawAcross(i, static_cast<uint8_t>(covered * 255.0f));
        }
    }

public:
    static constexpr InputCapability kRequired = InputCapability::Audio;

    void on_init() override {
        set_fps(kFps);
        _analyzer.reset();
        _level = 0.0f;
        _hue = 0.0f;
    }

    void on_update(const InputSnapshot &input) override {
        if (_analyzer.update(input.audio)) {
            // Размах задаёт средняя энергия полос: по максимуму слайдер
            // упирался бы в края почти всё время. Цвет берём от самой
            // громкой полосы — на басу красный, на верхних холодный.
            float energy = 0.0f;
            float loudest = 0.0f;
            uint8_t loudestBand = 0;
            for (uint8_t i = 0; i < kBands; ++i) {
                const float value = _analyzer.value(i);
                energy += value;
                if (value > loudest) {
                    loudest = value;
                    loudestBand = i;
                }
            }
            energy /= static_cast<float>(kBands);

            float target = (energy - kQuietEnergy) / (kLoudEnergy - kQuietEnergy);
            if (target < 0.0f) target = 0.0f;
            if (target > 1.0f) target = 1.0f;

            // Цвет тянется к преобладающей полосе каждый кадр, а не только
            // на пиках: так переход между частотами получается плавным.
            updateHue(static_cast<uint8_t>(loudestBand * (256U / kBands)));

            if (target > _level) {
                _level = target;
            }
        }

        _level -= kDecay;
        if (_level < 0.0f) _level = 0.0f;

        // Кадр не стираем, а притушаем: край оставляет короткий след.
        LedMatrix.fader(kTrailFade);
        paint();
    }

    void on_update() override {}
};
