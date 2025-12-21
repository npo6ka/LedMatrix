#pragma once

#include "stdint.h"
#include "libs/coord.h"

#define START_LENGTH 4                      // начальная длина змейки
#define MAX_SNAKE_LENGTH LEDS_HEIGHT * LEDS_WIDTH     // максимальная длина змейки

#define COLOR_SNAKE CRGB(CRGB::Green)       // Цвет змейки
#define COLOR_APPLE CRGB(CRGB::Orange)      // Цвет яблока на поле

// static_assert(LEDS_HEIGHT / 2 >= START_LENGTH, "Low LEDS_HEIGHT size for starting snake mode");
