#pragma once

#include "FastLED.h"
#include "configs/constants.h"
#include "debug_lib.h"

static_assert(WIDTH < UINT8_MAX, "ERROR invalid WIDTH");
static_assert(HEIGHT < UINT8_MAX, "ERROR invalid HEIGHT");
