#include "led_matrix.h"

#include "configs/constants.h"
#include "fl/xymap.h"
#include "libs/debug_lib.h"

static_assert(LEDS_WIDTH <= LEDS_HW_WIDTH, "invalid LEDS_WIDTH");
static_assert(LEDS_HEIGHT <= LEDS_HW_HEIGHT, "invalid LEDS_HEIGHT");
static_assert(LEDS_HW_WIDTH < std::numeric_limits<index_t>::max(), "invalid LEDS_HW_WIDTH");
static_assert(LEDS_HW_HEIGHT < std::numeric_limits<index_t>::max(), "invalid LEDS_HW_HEIGHT");

#if (LEDS_CONNECTION_ANGLE == 0)
    #define POS_X x
    #define POS_Y (LEDS_HW_HEIGHT - y - 1)
#elif (LEDS_CONNECTION_ANGLE == 1)
    #define POS_X x
    #define POS_Y y
#elif (LEDS_CONNECTION_ANGLE == 2)
    #define POS_X (LEDS_HW_WIDTH - x - 1)
    #define POS_Y y
#elif (LEDS_CONNECTION_ANGLE == 3)
    #define POS_X (LEDS_HW_WIDTH - x - 1)
    #define POS_Y (LEDS_HW_HEIGHT - y - 1)
#else
    #define POS_X x
    #define POS_Y y
    #pragma message "Wrong matrix parameters! Set to default"
#endif

#if (LEDS_STRIP_DIRECTION == 0)
    #define THIS_X POS_X
    #define THIS_Y POS_Y
    #define THIS_H LEDS_HW_HEIGHT
#else
    #define THIS_X POS_Y
    #define THIS_Y POS_X
    #define THIS_H LEDS_HW_WIDTH
#endif

CLedMatrix LedMatrix;

static CRGB& get_dummy_pix() {
    static CRGB _dummy_pix;
    // гарантируем, что (!_dummy_pix) is true
    _dummy_pix.setRGB(0, 0, 0);
    return _dummy_pix;
}

static index_t get_pix_num(index_t x, index_t y) {
    // x - номер столбца (в циклах ассоциируется с LEDS_WIDTH)
    // y - номер строки (в циклах ассоциируется с LEDS_HEIGHT)
    // матрица нумеруется сверху вниз по y, слева направо по x
    // 00 01 02
    // 10 11 12
    // 20 21 22

    if (LEDS_MATRIX_TYPE || THIS_Y % 2 == 0) {
        return THIS_Y * THIS_H + THIS_X;
    } else {
        return THIS_Y * THIS_H + THIS_H - THIS_X - 1;
    }
}

CLedMatrix::CLedMatrix() {}

void CLedMatrix::setup() {
    FastLED.addLeds<WS2812B, LEDS_PIN, LEDS_COLOR_ORDER>(_leds, LEDS_HW_SIZE);
    FastLED.setCorrection(TypicalLEDStrip);
    FastLED.setMaxPowerInVoltsAndMilliamps(5, LEDS_MAX_POWER);
    FastLED.setBrightness(LEDS_BRIGHTNRSS);
    FastLED.clear();
}

CRGB& CLedMatrix::atUnsafe(size_t index) {
#if LEDS_WIDTH == LEDS_HW_WIDTH && LEDS_HEIGHT == LEDS_HW_HEIGHT
    return _leds[index];
#else
    return atUnsafe(index % LEDS_WIDTH, index / LEDS_WIDTH);
#endif
}

CRGB& CLedMatrix::atUnsafe(index_t x, index_t y) {
    return _leds[get_pix_num(x, y)];
}

CRGB& CLedMatrix::at(size_t index) {
    if (index < 0 || index >= size()) {
        return get_dummy_pix();
    }

    return atUnsafe(index);
}

CRGB& CLedMatrix::at(index_t x, index_t y) {
    if (x < 0 || x >= width() || y < 0 || y >= height()) {
        return get_dummy_pix();
    }

    return atUnsafe(x, y);
}

void CLedMatrix::clear() {
    for (size_t i = 0; i < size(); ++i) {
        atUnsafe(i) = CRGB(0, 0, 0);
    }
}

void CLedMatrix::fader(uint8_t fadefactor) {
    for (size_t i = 0; i < size(); ++i) {
        atUnsafe(i).fadeToBlackBy(fadefactor);
    }
}

void CLedMatrix::drawLine(index_t x1, index_t y1, index_t x2, index_t y2, CRGB color) {
    // Рисование линии по Алгоритму Брезенхэма
    index_t deltaX = abs((int16_t)x2 - x1);
    index_t deltaY = abs((int16_t)y2 - y1);
    index_t x1_ = x1;
    index_t y1_ = y1;
    int8_t signX = x1_ < x2 ? 1 : -1;
    int8_t signY = y1_ < y2 ? 1 : -1;

    int16_t error = deltaX - deltaY;
    int32_t error2;

    at(x2, y2) = color;
    while (x1_ != x2 || y1_ != y2) {
        at(x1_, y1_) = color;
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

void CLedMatrix::drawRect(index_t x1, index_t y1, index_t x2, index_t y2, CRGB color) {
    if (x1 > x2) {
        std::swap(x1, x2);
    }
    if (y1 > y2) {
        std::swap(y1, y2);
    }

    for (index_t x = x1; x < x2; ++x) {
        for (index_t y = y1; y < y2; ++y) {
            at(x, y) = color;
        }
    }
}

void CLedMatrix::drawRectBorder(index_t x1, index_t y1, index_t x2, index_t y2, index_t s, CRGB color) {
    drawRect(x1 + 0, y1 + 0, x2 + 0, y1 + s, color);
    drawRect(x1 + 0, y2 - s, x2 + 0, y2 + 0, color);
    drawRect(x1 + 0, y1 + s, x1 + s, y2 - s, color);
    drawRect(x2 - s, y1 + s, x2 + 0, y2 - s, color);
}
