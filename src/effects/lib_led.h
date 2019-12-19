#pragma once

#define DEBUG true

#include "FastLED.h"
#include "debug_lib.h"

#define DATA_PIN       (2U)    // номер порта к которому подключены светодиоды

#define WIDTH          16
#define HEIGHT         16
#define LEDS_CNT       WIDTH * HEIGHT
#define CURRENT_LIMIT  (2000U) // лимит по току в миллиамперах, автоматически управляет яркостью (пожалей свой блок питания!) 0 - выключить лимит

#define MATRIX_TYPE           (0U)                          // тип матрицы: 0 - зигзаг, 1 - параллельная
#define CONNECTION_ANGLE      (2U)                          // угол подключения матрицы (0-3): 0 - левый нижний, 1 - левый верхний, 2 - правый верхний, 3 - правый нижний
                                                            // 1---------2
                                                            // -----------
                                                            // -----------
                                                            // -----------
                                                            // 0---------3
#define STRIP_DIRECTION       (0U)                          // направление ленты из угла (0-1): 0 - горизонтальное (из углов влево или вправо)
                                                            // 1>>>------2  1------<<<2  1---------2  1---------2
                                                            // -----------  -----------  -----------  -----------
                                                            // -----------  -----------  -----------  -----------
                                                            // -----------  -----------  -----------  -----------
                                                            // 0---------3  0---------3  0>>>------3  0------<<<3

                                                            // 1 - Вертикальное (из углов вверх или вниз)
                                                            // 1---------2  1---------2  1---------2  1---------2
                                                            // v----------  ----------v  -----------  -----------
                                                            // v----------  ----------v  ^----------  ----------^
                                                            // -----------  -----------  ^----------  ----------^
                                                            // 0---------3  0---------3  0---------3  0---------3


// ************* НАСТРОЙКА МАТРИЦЫ *****
//hooks for old vertion
#if (STRIP_DIRECTION == 2)
#define STRIP_DIRECTION 0
#endif
#if (STRIP_DIRECTION == 3)
#define STRIP_DIRECTION 1
#endif

#if (CONNECTION_ANGLE == 0 && STRIP_DIRECTION == 0)
#define _WIDTH WIDTH
#define THIS_X (HEIGHT - x - 1)
#define THIS_Y y

#elif (CONNECTION_ANGLE == 0 && STRIP_DIRECTION == 1)
#define _WIDTH HEIGHT
#define THIS_X y
#define THIS_Y (HEIGHT - x - 1)

#elif (CONNECTION_ANGLE == 1 && STRIP_DIRECTION == 0)
#define _WIDTH WIDTH
#define THIS_X x
#define THIS_Y y

#elif (CONNECTION_ANGLE == 1 && STRIP_DIRECTION == 1)
#define _WIDTH HEIGHT
#define THIS_X y
#define THIS_Y x

#elif (CONNECTION_ANGLE == 2 && STRIP_DIRECTION == 0)
#define _WIDTH WIDTH
#define THIS_X y
#define THIS_Y (WIDTH - y - 1)

#elif (CONNECTION_ANGLE == 2 && STRIP_DIRECTION == 1)
#define _WIDTH HEIGHT
#define THIS_X (WIDTH - y - 1)
#define THIS_Y (HEIGHT - x - 1)

#elif (CONNECTION_ANGLE == 3 && STRIP_DIRECTION == 0)
#define _WIDTH WIDTH
#define THIS_X (HEIGHT - x - 1) 
#define THIS_Y (WIDTH - y - 1)

#elif (CONNECTION_ANGLE == 3 && STRIP_DIRECTION == 1)
#define _WIDTH HEIGHT
#define THIS_X (WIDTH - y - 1) 
#define THIS_Y x

#else
#define _WIDTH WIDTH
#define THIS_X x
#define THIS_Y y
#pragma message "Wrong matrix parameters! Set to default"

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
    FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, LEDS_CNT).setCorrection(TypicalLEDStrip);
    FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT);
    FastLED.clear();

    pinMode(LED_BUILTIN, OUTPUT);
}

// получить номер пикселя в ленте по координатам
static uint16_t getPixNum(const uint8_t x, const uint8_t y)
{
    // x - номер строки
    // y - номер столбца
    // матрица нумеруется сверху вниз по x, слева направо по y (как все массивы в си)
    if (x < 0 || x >= HEIGHT || y < 0 || y >= WIDTH) {
        out("Value out of range in function getPixNum %d %d\n", x, y);
        return 0;
    }

    if (
#if (STRIP_DIRECTION == 0)
    x % 2 == 0
#else
    y % 2 == 0
#endif
    || MATRIX_TYPE)                             // если чётная строка
    {
        return (THIS_Y * _WIDTH + THIS_X);
    } else {                                    // если нечётная строка
        return (THIS_Y * _WIDTH + _WIDTH - THIS_X - 1);
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
    for (uint8_t i = 0; i < WIDTH; i++) {
        for (uint8_t j = 0; j < HEIGHT; j++) {
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

    setPixColor(x2, y2, color);
    while (x1 != x2 || y1 != y2) {
        setPixColor(x1, y1, color);
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
