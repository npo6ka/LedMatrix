#pragma once

#include "effects/effect.h"
#include "effects/effects_impl/music/band_levels.h"

class MusicRunner : public Effect {
    static constexpr uint8_t kBands = MusicBandLevels::kBands;
    static constexpr uint8_t kFps = 60;
    static constexpr uint8_t kMaxSparks = 16;

    // Комета рождается на ударе, а следующая в той же полосе — только после
    // того, как её громкость снова опустится: без этого громкое место
    // сыпало бы кометами каждый кадр.
    static constexpr float kFireLevel = 0.8f;
    static constexpr float kRearmLevel = 0.6f;
    // Минимальный промежуток между запусками, чтобы плотный бит не
    // превращался в сплошную кашу из комет.
    static constexpr uint32_t kMinSpawnGapMs = 120;

    // Пробег от начала ленты до конца: слабый удар тянется полторы секунды,
    // сильный проскакивает за полсекунды.
    static constexpr float kSlowRunSeconds = 1.6f;
    static constexpr float kFastRunSeconds = 0.5f;

    // Насколько притухает прошлый кадр — этим и рисуется шлейф.
    static constexpr uint8_t kTrailFade = 28;

    struct Spark {
        float pos = 0.0f;
        float speed = 0.0f;
        uint8_t hue = 0;
        uint8_t band = 0;
        bool alive = false;
    };

    MusicBandLevels _analyzer;
    Spark _sparks[kMaxSparks];
    bool _armed[kBands] = {};
    uint32_t _lastSpawnMs = 0;

    index_t runLength() const {
        const index_t width = LedMatrix.width();
        const index_t height = LedMatrix.height();
        return (width <= 1) ? height : width;
    }

    // Комета занимает всю поперечину матрицы, а не одну строку: на ленте это
    // просто пиксель, на матрице — столбец целиком.
    void drawAcross(index_t along, uint8_t hue, uint8_t amount) {
        if (amount == 0 || along >= runLength()) return;

        const CRGB color = CHSV(hue, 255, 255);
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

    void spawn(uint8_t band, float value) {
        Spark *slot = nullptr;
        for (Spark &spark : _sparks) {
            if (!spark.alive) {
                slot = &spark;
                break;
            }
        }
        // Свободных слотов нет — удар просто пропускаем, иначе пришлось бы
        // гасить чужую комету на середине пробега.
        if (!slot) return;

        float excess = (value - kFireLevel) / (1.0f - kFireLevel);
        if (excess < 0.0f) excess = 0.0f;
        if (excess > 1.0f) excess = 1.0f;
        const float seconds =
            kSlowRunSeconds + (kFastRunSeconds - kSlowRunSeconds) * excess;

        slot->alive = true;
        slot->pos = 0.0f;
        slot->band = band;
        slot->hue = static_cast<uint8_t>(band * (256U / kBands));
        slot->speed = static_cast<float>(runLength())
            / (seconds * static_cast<float>(kFps));
    }

    void move() {
        const float length = static_cast<float>(runLength());
        for (Spark &spark : _sparks) {
            if (!spark.alive) continue;
            spark.pos += spark.speed;
            if (spark.pos >= length) spark.alive = false;
        }
    }

    void draw() {
        // Идём от верхних полос к нижним: низкие частоты рисуются последними
        // и перекрывают остальные там, где кометы встретились.
        for (uint8_t band = kBands; band > 0; --band) {
            for (const Spark &spark : _sparks) {
                if (!spark.alive || spark.band != band - 1U) continue;

                const index_t whole = static_cast<index_t>(spark.pos);
                const float frac = spark.pos - static_cast<float>(whole);
                // Комета размазана по двум соседним пикселям: на медленном
                // пробеге иначе видно, как она скачет по светодиодам.
                const uint8_t nearAmount =
                    static_cast<uint8_t>((1.0f - frac) * 255.0f);
                const uint8_t farAmount = static_cast<uint8_t>(frac * 255.0f);
                drawAcross(whole, spark.hue, nearAmount);
                drawAcross(static_cast<index_t>(whole + 1U), spark.hue, farAmount);
            }
        }
    }

public:
    static constexpr InputCapability kRequired = InputCapability::Audio;

    void on_init() override {
        set_fps(kFps);
        _analyzer.reset();
        _lastSpawnMs = 0;
        for (Spark &spark : _sparks) {
            spark = Spark();
        }
        for (uint8_t i = 0; i < kBands; ++i) {
            _armed[i] = true;
        }
    }

    void on_update(const InputSnapshot &input) override {
        if (_analyzer.update(input.audio)) {
            // Полосы перебираем снизу вверх, поэтому при одновременном ударе
            // в нескольких диапазонах кометой выстрелит самый низкий.
            int8_t candidate = -1;
            for (uint8_t i = 0; i < kBands; ++i) {
                const float value = _analyzer.value(i);
                if (_armed[i] && value >= kFireLevel) {
                    if (candidate < 0) candidate = static_cast<int8_t>(i);
                } else if (value <= kRearmLevel) {
                    _armed[i] = true;
                }
            }

            if (candidate >= 0 && input.frameMs - _lastSpawnMs >= kMinSpawnGapMs) {
                const uint8_t band = static_cast<uint8_t>(candidate);
                spawn(band, _analyzer.value(band));
                _armed[band] = false;
                _lastSpawnMs = input.frameMs;
            }
        }

        // Кадр не стираем, а притушаем: из остатков и складывается шлейф.
        LedMatrix.fader(kTrailFade);
        move();
        draw();
    }

    void on_update() override {}
};
