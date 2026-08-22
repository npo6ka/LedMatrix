#pragma once

#include "effects/effect.h"
#include "effects/effects_impl/music/band_levels.h"

class MusicComet : public Effect {
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

    // Длина головы кометы в пикселях и яркость её последнего пикселя.
    static constexpr uint8_t kHeadLength = 4;
    static constexpr uint8_t kHeadTailAmount = 96;

    // Насколько притухает прошлый кадр — этим и рисуется хвост.
    // Гасим медленнее, чем в узком режиме: у широкой головы и след длиннее.
    static constexpr uint8_t kTrailFade = 18;

    struct Spark {
        float pos = 0.0f;
        float prevPos = 0.0f;
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

    // Яркость пикселя тела кометы: нос полный, дальше плавно темнеет к хвосту.
    static uint8_t bodyAmount(int32_t offset, int32_t length) {
        if (length <= 1) return 255;
        const float t = static_cast<float>(offset) / static_cast<float>(length - 1);
        return static_cast<uint8_t>(
            255.0f - (255.0f - static_cast<float>(kHeadTailAmount)) * t);
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
        slot->prevPos = 0.0f;
        slot->band = band;
        slot->hue = static_cast<uint8_t>(band * (256U / kBands));
        slot->speed = static_cast<float>(runLength())
            / (seconds * static_cast<float>(kFps));
    }

    void move() {
        const float length = static_cast<float>(runLength());
        for (Spark &spark : _sparks) {
            if (!spark.alive) continue;
            spark.prevPos = spark.pos;
            spark.pos += spark.speed;
            // Даём голове доехать до конца целиком, а не пропасть носом.
            if (spark.pos >= length + kHeadLength) spark.alive = false;
        }
    }

    void drawSpark(const Spark &spark) {
        const int32_t nose = static_cast<int32_t>(spark.pos);
        const int32_t tail = static_cast<int32_t>(spark.prevPos);

        // За кадр комета проходит несколько диодов, поэтому рисуем не точку,
        // а весь пройденный отрезок: иначе на ленте горят отдельные диоды
        // с шагом в скорость, а не сплошная полоса.
        int32_t length = nose - tail + kHeadLength;
        if (length < kHeadLength) length = kHeadLength;

        for (int32_t i = 0; i < length; ++i) {
            const int32_t pos = nose - i;
            if (pos < 0) break;
            drawAcross(static_cast<index_t>(pos), spark.hue, bodyAmount(i, length));
        }
    }

    void draw() {
        // Идём от верхних полос к нижним: низкие частоты рисуются последними
        // и перекрывают остальные там, где кометы встретились.
        for (uint8_t band = kBands; band > 0; --band) {
            for (const Spark &spark : _sparks) {
                if (!spark.alive || spark.band != band - 1U) continue;
                drawSpark(spark);
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
            // При одновременном ударе в нескольких диапазонах кометой
            // выстреливает самый громкий из них — он и задаёт цвет.
            int8_t candidate = -1;
            float candidateValue = 0.0f;
            for (uint8_t i = 0; i < kBands; ++i) {
                const float value = _analyzer.value(i);
                if (_armed[i] && value >= kFireLevel) {
                    if (candidate < 0 || value > candidateValue) {
                        candidate = static_cast<int8_t>(i);
                        candidateValue = value;
                    }
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

        // Кадр не стираем, а притушаем: из остатков и складывается хвост.
        LedMatrix.fader(kTrailFade);
        move();
        draw();
    }

    void on_update() override {}
};
