#pragma once

#include "FastLED.h"
//#include "constants_window.h"
#include "constants_matrix.h"
#include "debug_lib.h"

// ************* НАСТРОЙКА МАТРИЦЫ **************
//hooks for old vertion
#if (STRIP_DIRECTION == 2)
#   define STRIP_DIRECTION 0
#elif (STRIP_DIRECTION == 3)
#   define STRIP_DIRECTION 1
#endif

#if (STRIP_DIRECTION == 0)
#   define _WIDTH WIDTH
#elif (STRIP_DIRECTION == 1)
#   define _WIDTH HEIGHT
#endif

#if (CONNECTION_ANGLE == 0 && STRIP_DIRECTION == 0)
#   define THIS_X (HEIGHT - x - 1)
#   define THIS_Y y
#elif (CONNECTION_ANGLE == 0 && STRIP_DIRECTION == 1)
#   define THIS_X y
#   define THIS_Y (HEIGHT - x - 1)
#elif (CONNECTION_ANGLE == 1 && STRIP_DIRECTION == 0)
#   define THIS_X x
#   define THIS_Y y
#elif (CONNECTION_ANGLE == 1 && STRIP_DIRECTION == 1)
#   define THIS_X y
#   define THIS_Y x
#elif (CONNECTION_ANGLE == 2 && STRIP_DIRECTION == 0)
#   define THIS_X x
#   define THIS_Y (WIDTH - y - 1)
#elif (CONNECTION_ANGLE == 2 && STRIP_DIRECTION == 1)
#   define THIS_X (WIDTH - y - 1)
#   define THIS_Y x
#elif (CONNECTION_ANGLE == 3 && STRIP_DIRECTION == 0)
#   define THIS_X (HEIGHT - x - 1)
#   define THIS_Y (WIDTH - y - 1)
#elif (CONNECTION_ANGLE == 3 && STRIP_DIRECTION == 1)
#   define THIS_X (WIDTH - y - 1)
#   define THIS_Y (HEIGHT - x - 1)
#else
#   define _WIDTH WIDTH
#   define THIS_X x
#   define THIS_Y y
#   pragma message "Wrong matrix parameters! Set to default"
#endif

extern CRGB leds[LEDS_CNT];

// Получить номер пикселя по координатам
static uint16_t getPixNum(const uint8_t x, const uint8_t y);

// получить объект пикселя по координатам
static CRGB &getPix(int x, int y);

// получить 24-битный код цвета из объекта пикселя
static uint32_t getPixColor(CRGB val);

// получить 24-битный код цвета по координатам
static uint32_t getPixColor(int x, int y);

// УСТАРЕВШАЯ ФУНКЦИЯ. Лучше использовать getPix(x, y) = color
// установить цвет пикселя по координатам.
// Структура CRGB поддерживает автоматическую
// конвертацию из CHSV и из int значений.
// Т.е. допустимо в качестве параметра color передавать помимо CRGB объекта,
// CHSV объекты и uint32_t значения
static void setPixColor(int x, int y, CRGB color);

// Получить ссылку на матрицу светодиодов
// !!! Не рекомендуется записывать значения напрямую в этот массив
// так как значения в нем могут располагаться в различном порядке
// для различных типов матриц и возможных поворотах матриц на
// углы 90, 180, 270 градусов.
static CRGB* getLeds(void);

// Затемнить все светодиоды на матрице на указанный шаг
static void fader(uint8_t step);

// Затемнить светодиод с соответствующими координатами на матрице на указанный шаг
static void fadePix(uint8_t x, uint8_t y, uint8_t step);

// Нарисовать линию на матрице по указанным координатам и соответствующим цветом
// x1, y1 - координаты 1 точки
// x2, y2 - координаты 2 точки
// color - цвет, которым будет нарисована линия
static void drawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, CRGB color);

/* инициализация библиотеки FastLed и
 * установка цветовой коррекции светодиодов
 */
static void led_setup()
{
    FastLED.addLeds<WS2812B, DATA_PIN, COLOR_ORDER>(leds, LEDS_CNT).setCorrection(TypicalLEDStrip);
    FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT);
    FastLED.clear();
}

// получить номер пикселя в ленте по координатам
static uint16_t getPixNum(const uint8_t x, const uint8_t y)
{
    // x - номер строки (в циклах ассоциируется с HEIGHT)
    // y - номер столбца (в циклах ассоциируется с WIDTH)
    // матрица нумеруется сверху вниз по x, слева направо по y (как все массивы в си)
    // 00 01 02
    // 10 11 12
    // 20 21 22
    if (x < 0 || x >= HEIGHT || y < 0 || y >= WIDTH) {
        out("Value out of range in function getPixNum %d %d\n", x, y);
        return 0;
    }

    if (THIS_X % 2 == 0 || MATRIX_TYPE) {       // если чётная строка
        return (THIS_X * _WIDTH + THIS_Y);
    } else {                                    // если нечётная строка
        return (THIS_X * _WIDTH + _WIDTH - THIS_Y - 1);
    }
}

static CRGB &getPix(int x, int y) {
    if (x < 0 || x >= HEIGHT || y < 0 || y >= WIDTH) {
        out("Value out of range in function getPix %d %d\n", x, y);
        return leds[0];
    }

    return leds[getPixNum(x, y)];
}

static uint32_t getPixColor(CRGB val) {
    return (((uint32_t)val.r << 16) | ((uint32_t)val.g << 8 ) | val.b);
}

static uint32_t getPixColor(int x, int y) {
    if (x < 0 || x >= HEIGHT || y < 0 || y >= WIDTH) {
        out("Value out of range in function getPixColor %d %d\n", x, y);
        return 0;
    }

    return getPixColor(getPix(x, y));
}

static void setPixColor(int x, int y, CRGB color) {
    if (x < 0 || x >= HEIGHT || y < 0 || y >= WIDTH) {
        out("Value out of range in function setPixColor %d %d\n", x, y);

        return;
    }

    getPix(x, y) = color;
}

static CRGB* getLeds(void) {
    return leds;
}

static void fader(uint8_t step) {
    for (uint8_t i = 0; i < HEIGHT; i++) {
        for (uint8_t j = 0; j < WIDTH; j++) {
            fadePix(i, j, step);
        }
    }
}

static void fadePix(uint8_t x, uint8_t y, uint8_t step) {
    getPix(x, y).fadeToBlackBy(step);
}

static void drawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, CRGB color)
{
    // Рисование линии по Алгоритму Брезенхэма
    uint8_t deltaX = abs((int16_t)x2 - x1);
    uint8_t deltaY = abs((int16_t)y2 - y1);
    int8_t signX = x1 < x2 ? 1 : -1;
    int8_t signY = y1 < y2 ? 1 : -1;

    int16_t error = deltaX - deltaY;
    int32_t error2;

    getPix(x2, y2) = color;
    while (x1 != x2 || y1 != y2) {
        getPix(x1, y1) = color;
        error2 = error * 2;

        if (error2 > -deltaY) {
            error -= deltaY;
            x1 += signX;
        }
        if (error2 < deltaX) {
            error += deltaX;
            y1 += signY;
        }
    }
}
