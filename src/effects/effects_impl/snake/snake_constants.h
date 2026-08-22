#pragma once

#include "stdint.h"
#include "libs/coord.h"

#define START_LENGTH 4                      // начальная длина змейки
#define MAX_SNAKE_LENGTH ((LEDS_SIZE) < 2 ? 2 : (LEDS_SIZE))     // максимальная длина змейки (минимум 2 для очереди)

#define COLOR_SNAKE CRGB(CRGB::Green)       // Цвет змейки
#define COLOR_APPLE CRGB(CRGB::Orange)      // Цвет яблока на поле
