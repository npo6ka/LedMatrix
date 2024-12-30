#include "lib_led.h"

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

/* инициализация библиотеки FastLed и
 * установка цветовой коррекции светодиодов
 */
void led_setup(void)
{
    FastLED.addLeds<WS2812B, LED_PIN, COLOR_ORDER>(leds, LEDS_CNT).setCorrection(TypicalLEDStrip);
    FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT);
    FastLED.setBrightness(LED_BRIGHTNRSS);
    FastLED.clear();
}

// получить номер пикселя в ленте по координатам
static uint16_t getPixNum(uint16_t x, uint16_t y)
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

CRGB &getPix(const int x, const int y) {
    if (x < 0 || x >= HEIGHT || y < 0 || y >= WIDTH) {
        out("Value out of range in function getPix %d %d\n", x, y);
        return leds[0];
    }

    return leds[getPixNum(x, y)];
}

CRGB* getLeds(void) {
    return leds;
}

void fader(uint8_t step) {
    for (uint8_t i = 0; i < HEIGHT; i++) {
        for (uint8_t j = 0; j < WIDTH; j++) {
            fadePix(i, j, step);
        }
    }
}

void fadePix(uint16_t x, uint16_t y, uint8_t step) {
    getPix(x, y).fadeToBlackBy(step);
}

void drawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, CRGB color)
{
    // Рисование линии по Алгоритму Брезенхэма
    uint16_t deltaX = abs((int16_t)x2 - x1);
    uint16_t deltaY = abs((int16_t)y2 - y1);
    uint16_t x1_ = x1;
    uint16_t y1_ = y1;
    int8_t signX = x1_ < x2 ? 1 : -1;
    int8_t signY = y1_ < y2 ? 1 : -1;

    int32_t error = deltaX - deltaY;
    int32_t error2;

    getPix(x2, y2) = color;
    while (x1_ != x2 || y1_ != y2) {
        getPix(x1_, y1_) = color;
        error2 = error * 2;

        if (error2 > -deltaY) {
            error -= deltaY;
            x1_ += signX;
        }
        if (error2 < deltaX) {
            error += deltaX;
            y1_ += signY;
        }
    }
}
