#pragma once

#include <limits>

#include "FastLED.h"

#include "configs/constants.h"
#include "libs/debug_lib.h"
#include "libs/range.h"

uint16_t XY(uint8_t x, uint8_t y);

class CLedMatrix {
public:
    CLedMatrix();

    void setup();

    // Получить количество пикселей в матрице
    size_t size() const { return LEDS_SIZE; }

    // Получить ширину матрицы
    index_t width() const { return LEDS_WIDTH; }

    // Получить высоту матрицы
    index_t height() const { return LEDS_HEIGHT; }

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

private:
    CRGB _leds[LEDS_HW_SIZE];
};

extern CLedMatrix LedMatrix;
