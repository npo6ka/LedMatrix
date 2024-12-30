#include "led_matrix.h"

#include "configs/constants.h"
#include "libs/debug_lib.h"

#if (CONNECTION_ANGLE == 0)
#   define POS_X x
#   define POS_Y (HEIGHT - y - 1)
#elif (CONNECTION_ANGLE == 1)
#   define POS_X x
#   define POS_Y y
#elif (CONNECTION_ANGLE == 2)
#   define POS_X (WIDTH - x - 1)
#   define POS_Y y
#elif (CONNECTION_ANGLE == 3)
#   define POS_X (WIDTH - x - 1)
#   define POS_Y (HEIGHT - y - 1)
#else
#   define POS_X x
#   define POS_Y y
#   pragma message "Wrong matrix parameters! Set to default"
#endif

#if (STRIP_DIRECTION == 0)
#   define THIS_X POS_X
#   define THIS_Y POS_Y
#   define THIS_H HEIGHT
#else
#   define THIS_X POS_Y
#   define THIS_Y POS_X
#   define THIS_H WIDTH
#endif

CLedMatrix LedMatrix;

static CRGB& get_dummy_pix() {
    static CRGB _dummy_pix;
    // гарантируем, что (!_dummy_pix) is true
    _dummy_pix.setRGB(0, 0, 0);
    return _dummy_pix;
}

static size_t get_pix_num(index_t x, index_t y)
{
    // x - номер столбца (в циклах ассоциируется с WIDTH)
    // y - номер строки (в циклах ассоциируется с HEIGHT)
    // матрица нумеруется сверху вниз по y, слева направо по x
    // 00 01 02
    // 10 11 12
    // 20 21 22

    if (MATRIX_TYPE || THIS_Y % 2 == 0) {
        return THIS_Y * THIS_H + THIS_X;
    } else {
        return THIS_Y * THIS_H + THIS_H - THIS_X - 1;
    }
}

void CLedMatrix::setup() {
    FastLED.addLeds<WS2812B, DATA_PIN, COLOR_ORDER>(_leds, size());
    FastLED.setCorrection(TypicalLEDStrip);
    FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT);
    FastLED.setBrightness(LED_BRIGHTNRSS);
    FastLED.clear();
}

CRGB& CLedMatrix::at(size_t index) {
    if (index < 0 || index >= size()) {
        return get_dummy_pix();
    }

    return _leds[index];
}

CRGB& CLedMatrix::at(index_t x, index_t y) {
    if (x < 0 || x >= width() || y < 0 || y >= height()) {
        return get_dummy_pix();
    }

    return _leds[get_pix_num(x, y)];
}

void CLedMatrix::clear() {
    for (auto& pix : *this) {
        pix = CRGB(0, 0, 0);
    }
}

void CLedMatrix::fader(uint8_t fadefactor) {
    for (auto& pix : *this) {
        pix.fadeToBlackBy(fadefactor);
    }
}

void CLedMatrix::draw_line(index_t x1, index_t y1, index_t x2, index_t y2, CRGB color) {
    // Рисование линии по Алгоритму Брезенхэма
    if (x1 > x2) std::swap(x1, x2);
    if (y1 > y2) std::swap(y1, y2);

    index_t deltaX = abs(x2 - x1);
    index_t deltaY = abs(y2 - y1);
    index_t x1_ = x1;
    index_t y1_ = y1;
    int32_t error = deltaX - deltaY;
    int32_t error2;

    at(x2, y2) = color;
    while (x1_ != x2 || y1_ != y2) {
        at(x1_, y1_) = color;
        error2 = error * 2;

        if (error2 > -deltaY) {
            error -= deltaY;
            x1_ += 1;
        }
        if (error2 < deltaX) {
            error += deltaX;
            y1_ += 1;
        }
    }
}

void CLedMatrix::draw_rect(index_t x1, index_t y1, index_t x2, index_t y2, CRGB color) {
    if (x1 > x2) std::swap(x1, x2);
    if (y1 > y2) std::swap(y1, y2);

    for (index_t x = x1; x < x2; ++x) {
        for (index_t y = y1; y < y2; ++y) {
            at(x, y) = color;
        }
    }
}

void CLedMatrix::draw_border(index_t x1, index_t y1, index_t x2, index_t y2, index_t s, CRGB color) {
    draw_rect(x1 + 0, y1 + 0, x2 + 0, y1 + s, color);
    draw_rect(x1 + 0, y2 - s, x2 + 0, y2 + 0, color);
    draw_rect(x1 + 0, y1 + s, x1 + s, y2 - s, color);
    draw_rect(x2 - s, y1 + s, x2 + 0, y2 - s, color);
}
