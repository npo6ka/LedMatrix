#pragma once
// #include "window.h"
// #include "matrix.h"
#include "tv.h"

#include "stdint.h"

using index_t = uint16_t;

#ifndef LEDS_HW_WIDTH
// Реальная ширина матрицы сетодиодов.
// Можно переопределить для эмуляции ширины меньше действительной.
#define LEDS_HW_WIDTH LEDS_WIDTH
#endif

#ifndef LEDS_HW_HEIGHT
// Реальная высота матрицы сетодиодов.
// Можно переопределить для эмуляции высоты меньше действительной.
#define LEDS_HW_HEIGHT LEDS_HEIGHT
#endif

#define LEDS_HW_SIZE (LEDS_HW_WIDTH * LEDS_HW_HEIGHT)
#define LEDS_SIZE (LEDS_WIDTH * LEDS_HEIGHT)
