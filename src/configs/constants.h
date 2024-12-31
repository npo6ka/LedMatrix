#pragma once
// #include "window.h"
#include "matrix.h"

#include "stdint.h"

using index_t = uint16_t;

#ifndef LEDS_HW_WIDTH
#define LEDS_HW_WIDTH LEDS_WIDTH
#endif

#ifndef LEDS_HW_HEIGHT
#define LEDS_HW_HEIGHT LEDS_HEIGHT
#endif

#define LEDS_HW_SIZE (LEDS_HW_WIDTH * LEDS_HW_WIDTH)
#define LEDS_SIZE (LEDS_WIDTH * LEDS_HEIGHT)
