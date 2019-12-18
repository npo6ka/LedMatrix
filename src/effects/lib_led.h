#pragma once

#define DEBUG true

#include "FastLED.h"
#include "debug_lib.h"

#define DATA_PIN       (2U)    // номер порта к которому подключены светодиоды

#define WIDTH          16
#define HEIGHT         16
#define LEDS_CNT       WIDTH * HEIGHT
#define MAX_BRIGHTNESS 255
#define MAX_HSV        255
#define CURRENT_LIMIT  (2000U) // лимит по току в миллиамперах, автоматически управляет яркостью (пожалей свой блок питания!) 0 - выключить лимит

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


#define MATRIX_TYPE           (0U)                          // тип матрицы: 0 - зигзаг, 1 - параллельная
#define CONNECTION_ANGLE      (1U)                          // угол подключения: 0 - левый нижний, 1 - левый верхний, 2 - правый верхний, 3 - правый нижний
#define STRIP_DIRECTION       (0U)                          // направление ленты из угла: 0 - вправо, 1 - вверх, 2 - влево, 3 - вниз
                                                            // при неправильной настройке матрицы вы получите предупреждение "Wrong matrix parameters! Set to default"
                                                            // шпаргалка по настройке матрицы здесь! https://alexgyver.ru/matrix_guide/


// ************* НАСТРОЙКА МАТРИЦЫ *****
#if (CONNECTION_ANGLE == 0 && STRIP_DIRECTION == 0)
#define _WIDTH WIDTH
#define THIS_X x
#define THIS_Y y

#elif (CONNECTION_ANGLE == 0 && STRIP_DIRECTION == 1)
#define _WIDTH HEIGHT
#define THIS_X y
#define THIS_Y x

#elif (CONNECTION_ANGLE == 1 && STRIP_DIRECTION == 0)
#define _WIDTH WIDTH
#define THIS_X x
#define THIS_Y (HEIGHT - y - 1)

#elif (CONNECTION_ANGLE == 1 && STRIP_DIRECTION == 3)
#define _WIDTH HEIGHT
#define THIS_X (HEIGHT - y - 1)
#define THIS_Y x

#elif (CONNECTION_ANGLE == 2 && STRIP_DIRECTION == 2)
#define _WIDTH WIDTH
#define THIS_X (WIDTH - x - 1)
#define THIS_Y (HEIGHT - y - 1)

#elif (CONNECTION_ANGLE == 2 && STRIP_DIRECTION == 3)
#define _WIDTH HEIGHT
#define THIS_X (HEIGHT - y - 1)
#define THIS_Y (WIDTH - x - 1)

#elif (CONNECTION_ANGLE == 3 && STRIP_DIRECTION == 2)
#define _WIDTH WIDTH
#define THIS_X (WIDTH - x - 1)
#define THIS_Y y

#elif (CONNECTION_ANGLE == 3 && STRIP_DIRECTION == 1)
#define _WIDTH HEIGHT
#define THIS_X y
#define THIS_Y (WIDTH - x - 1)

#else
#define _WIDTH WIDTH
#define THIS_X x
#define THIS_Y y
#pragma message "Wrong matrix parameters! Set to default"

#endif

// получить номер пикселя в ленте по координатам
static uint16_t getPixNum(const uint8_t x, const uint8_t y)
{
    //if (x % 2 == 0) return x * WIDTH + WIDTH - y - 1;
    //else return x * WIDTH + y;
  if ((THIS_Y % 2 == 0) || MATRIX_TYPE)                     // если чётная строка
  {
    return (THIS_Y * _WIDTH + THIS_X);
  }
  else                                                      // если нечётная строка
  {
    return (THIS_Y * _WIDTH + _WIDTH - THIS_X - 1);
  }
}
/*
#define THIS_X (WIDTH - x - 1)
#define THIS_Y (HEIGHT - y - 1)


0, 0
(HEIGHT - x - 1) * WIDTH + y                   (0,2,4...) H(x)
(HEIGHT - x - 1) * WIDTH + WIDTH - y - 1       (1,3,5...) H(x)

0, 1
y * HEIGHT + (HEIGHT - x - 1) (0,2,4...) W(y)
y * HEIGHT + x                (1,3,5...) W(y)

1, 0
x * WIDTH + y                   (0,2,4...) H(x)
x * WIDTH + WIDTH - y - 1       (1,3,5...) H(x)


1, 3
y * HEIGHT + x                (0,2,4...) W(y)
y * HEIGHT + (HEIGHT - x - 1) (1,3,5...) W(y)

2, 2
x * WIDTH + WIDTH - y - 1       (0,2,4...) H(x)
x * WIDTH + y                   (1,3,5...) H(x)

2, 3
(WIDTH - y - 1) * HEIGHT + (HEIGHT - x - 1) (0,2,4...) W(y)
(WIDTH - y - 1) * HEIGHT + x                (1,3,5...) W(y)

3, 1
(WIDTH - y - 1) * HEIGHT + x                (0,2,4...) W(y)
(WIDTH - y - 1) * HEIGHT + (HEIGHT - x - 1) (1,3,5...) W(y)

3, 2
(HEIGHT - x - 1) * WIDTH + WIDTH - y - 1    (0,2,4...) H(x)
(HEIGHT - x - 1) * WIDTH + y                (1,3,5...) H(x)






*/
/*
static uint16_t getPixNum(int x, int y) {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
        Serial.print("Value out of range in function getPixNum");
        Serial.print(x);
        Serial.println(y);
        return 0;
    }

    //для тестов на эмуляторе необходим данный вариант:
    //return WIDTH * HEIGHT - (y * WIDTH + x + 1);

    return WIDTH * HEIGHT - (y & 0x1 ? (y * WIDTH + x + 1) : ((y + 1) * WIDTH - x));
  /* текущая матрица в рабочей железке
      x   0   1   2   3   4   5   6   7   8   9
    y
    0     99  98  97  96  95  94  93  92  91  90
    1     80  81  82  83  84  85  86  87  88  89
    2     79  78  77  76  75  74  73  72  71  70
    3     60  61  62  63  64  65  66  67  68  69
    4     59  58  57  56  55  54  53  52  51  50
    5     40  41  42  43  44  45  46  47  48  49
    6     39  38  37  36  35  34  33  32  31  30
    7     20  21  22  23  24  25  26  27  28  29
    8     19  18  17  16  15  14  13  12  11  10
  ->9     0   1   2   3   4   5   6   7   8   9 
  ^
входной провод
  
}*/

static CRGB &getPix(int x, int y) {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
        Serial.print("Value out of range in function getPix ");
        Serial.print(x);
        Serial.print(" ");
        Serial.println(y);
        return leds[0];
    }

    return leds[getPixNum(x, y)];
}

static uint32_t getPixColor(CRGB val) {
    return (((uint32_t)val.r << 16) | ((uint16_t)val.g << 8 ) | val.b);
}

static uint32_t getPixColor(int x, int y) {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
        Serial.print("Value out of range in function getPixColor ");
        Serial.print(x);
        Serial.print(" ");
        Serial.println(y);
        return 0;
    }

    return getPixColor(getPix(x, y));
}

static void setPixColor(int x, int y, CRGB color) {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) {
        Serial.print("Value out of range in function setPixColor ");
        Serial.print(x);
        Serial.print(" ");
        Serial.println(y);
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
