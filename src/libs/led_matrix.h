#pragma once

#include <limits>

#include "FastLED.h"

#include "configs/constants.h"
#include "libs/debug_lib.h"
#include "libs/range.h"

uint16_t XY(uint8_t x, uint8_t y);

class CLedMatrix {
public:
    // Ось, вдоль которой матрица складывается пополам в симметричном режиме
    enum class SymmetryAxis : uint8_t { X, Y };

    CLedMatrix();

    void setup();

    // Потолок FPS по протоколу WS2812: ~30 мкс/диод + latch.
    // Выше лента физически не успевает принять кадр.
    static uint16_t hardwareMaxFps() {
        const uint32_t minShowUs =
            static_cast<uint32_t>(LEDS_HW_SIZE) * 30U + 50U;
        const uint16_t maxFps = static_cast<uint16_t>(1000000U / minShowUs);
        return maxFps > 0 ? maxFps : 1;
    }

    uint8_t getBrightness() const;
    void setBrightness(uint8_t val);

    // Симметричный режим: логическая матрица вдвое меньше физической, а нарисованная
    // половина зеркально отражается на вторую перед отправкой кадра на ленту.
    // Логическая координата 0 оказывается на обоих краях, последняя — в центре.
    bool isSymmetric() const { return _symmetric; }
    void setSymmetric(bool enable);

    // Матрица складывается вдоль длинной стороны
    static constexpr SymmetryAxis symmetryAxis() {
        return LEDS_WIDTH >= LEDS_HEIGHT ? SymmetryAxis::X : SymmetryAxis::Y;
    }

    // Отразить нарисованную половину на вторую.
    // Вызывает движок эффектов перед FastLED.show(), эффектам трогать не нужно.
    void mirror();

    // Получить количество пикселей в матрице
    size_t size() const { return _size; }

    // Получить ширину матрицы
    index_t width() const { return _width; }

    // Получить высоту матрицы
    index_t height() const { return _height; }

    // Получить пиксель по порядковому индексы (UB в случае выхода за границы)
    CRGB& atUnsafe(size_t index);

    // Получить пиксель по координатам (UB в случае выхода за границы)
    CRGB& atUnsafe(index_t x, index_t y);

    // Получить пиксель по порядковому индексы
    CRGB& at(size_t index);

    // Получить пиксель по координатам
    CRGB& at(index_t x, index_t y);

    // Получить указатель на матрицу светодиодов
    // !!! Не рекомендуется записывать значения напрямую в этот массив
    // так как значения в нем могут располагаться в различном порядке
    // для различных типов матриц и возможных поворотах матриц на
    // углы 90, 180, 270 градусов.
    CRGB* leds() { return _leds; };

    // Очистить все светодиоды на матрице
    void clear();

    // Затемнить все светодиоды на матрице на указанный шаг
    void fader(uint8_t step);

    // Размытие соседей (как FastLED blur2d), без лимита 255 по ширине/высоте.
    // Соседи берутся по визуальной сетке at(x, y), а не по порядку проводов.
    void blur(uint8_t amount);

    // Пиксель по линейному индексу вдоль длинной стороны (для ленты 1×N / N×1)
    CRGB& atLinear(int pos);

    // Нарисовать линию по указанным координатам и соответствующим цветом
    // x1, y1 - координаты 1 точки
    // x2, y2 - координаты 2 точки
    // color - цвет, которым будет нарисована линия
    void drawLine(index_t x1, index_t y1, index_t x2, index_t y2, CRGB color);

    // Нарисовать заполненный прямоугольник по указанным координатам и соответствующим цветом
    // x1, y1 - координаты левой верхне точки
    // x2, y2 - координаты правой нижней точки (невключительно!)
    // color - цвет, которым будет нарисована линия
    void drawRect(index_t x1, index_t y1, index_t x2, index_t y2, CRGB color);

    // Нарисовать незаполненный прямоугольник по указанным координатам и соответствующим цветом
    // x1, y1 - координаты левой верхне точки
    // x2, y2 - координаты правой нижней точки (невключительно!)
    // size - размер бордюра
    // color - цвет, которым будет нарисована линия
    void drawRectBorder(index_t x1, index_t y1, index_t x2, index_t y2, index_t size, CRGB color);

    template <index_t W, index_t H>
    void drawSprite(index_t x, index_t y, const uint32_t* sprite) {
        for (index_t j = 0 ; j < H ; ++j) {
            for (index_t i = 0 ; i < W ; ++i) {
                at(x + i, y + j) = CRGB(pgm_read_dword(sprite++));
            }
        }
    }

    template <index_t W, index_t H>
    void drawSprite(index_t x, index_t y, const uint32_t (&sprite)[H][W]) {
        drawSprite<W, H>(x, y, &sprite[0][0]);
    }

    // Сжать спрайт в линию, если матрица шириной или высотой 1, и сдвинуть вдоль ленты
    template <index_t W, index_t H>
    void drawSpriteProjected(int origin, const uint32_t* sprite) {
        if (width() > 1 && height() > 1) {
            drawSprite<W, H>((index_t)origin, 0, sprite);
            return;
        }
        if (width() <= 1) {
            for (index_t j = 0; j < H; ++j) {
                CRGB acc(0, 0, 0);
                for (index_t i = 0; i < W; ++i) {
                    CRGB p(pgm_read_dword(sprite + j * W + i));
                    if (p) acc = p;
                }
                if (acc) {
                    at(0, origin + (int)j) = acc;
                }
            }
        } else {
            for (index_t i = 0; i < W; ++i) {
                CRGB acc(0, 0, 0);
                for (index_t j = 0; j < H; ++j) {
                    CRGB p(pgm_read_dword(sprite + j * W + i));
                    if (p) acc = p;
                }
                if (acc) {
                    at(origin + (int)i, 0) = acc;
                }
            }
        }
    }

    // range по ширине матрицы для range-based-циклов
    // left - левая граница (включительно)
    // right - правая граница (невключительно)
    // right (отрицательное) - отступ от ширины
    Range<index_t> rangeX(index_t left = 0, int right = -1) const {
        return rangeImpl(left, right, width());
    }

    // range по высоте матрицы для range-based-циклов
    // left - левая граница (включительно)
    // right - правая граница (невключительно)
    // right (отрицательное) - отступ от высоты
    Range<index_t> rangeY(index_t left = 0, int right = -1) const {
        return rangeImpl(left, right, height());
    }

private:
    Range<index_t> rangeImpl(index_t left, int right, index_t limit) const {
        if (left < 0) left = 0;
        if (right >= limit) right = limit;
        else if (right < 0) right = limit + right + 1;
        return Range<index_t>(left, right);
    }

    // Пересчитать логические размеры под текущий режим симметрии
    void applyGeometry();

    uint8_t _brightness = LEDS_BRIGHTNRSS;
    bool _symmetric = LEDS_SYMMETRIC_DEF_STATE;
    index_t _width = LEDS_WIDTH;
    index_t _height = LEDS_HEIGHT;
    size_t _size = LEDS_SIZE;
    CRGB _leds[LEDS_HW_SIZE];
};

extern CLedMatrix LedMatrix;
