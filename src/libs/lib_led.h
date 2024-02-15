#pragma once

#include "FastLED.h"
#include "configs/constants.h"
#include "debug_lib.h"

static_assert(WIDTH < UINT8_MAX, "ERROR invalid WIDTH");
static_assert(HEIGHT < UINT8_MAX, "ERROR invalid HEIGHT");

// инициализация матрицы в библиотеке FastLED
void led_setup(void);

// получить объект пикселя по координатам
CRGB &getPix(int x, int y);

// Получить ссылку на матрицу светодиодов
// !!! Не рекомендуется записывать значения напрямую в этот массив
// так как значения в нем могут располагаться в различном порядке
// для различных типов матриц и возможных поворотах матриц на
// углы 90, 180, 270 градусов.
CRGB *getLeds(void);

// Затемнить все светодиоды на матрице на указанный шаг
void fader(uint8_t step);

// Затемнить светодиод с соответствующими координатами на матрице на указанный шаг
void fadePix(uint8_t x, uint8_t y, uint8_t step);

// Нарисовать линию на матрице по указанным координатам и соответствующим цветом
// x1, y1 - координаты 1 точки
// x2, y2 - координаты 2 точки
// color - цвет, которым будет нарисована линия
void drawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, CRGB color);