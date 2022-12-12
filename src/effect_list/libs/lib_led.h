#pragma once

#include "FastLED.h"
//#include "constants_window.h"
#include "constants_matrix.h"
#include "debug_lib.h"

// инициализация матрицы в библиотеке FastLED
void led_setup(void);

// получить объект пикселя по координатам
CRGB &getPix(const int &x, const int &y);

// Получить ссылку на матрицу светодиодов
// !!! Не рекомендуется записывать значения напрямую в этот массив
// так как значения в нем могут располагаться в различном порядке
// для различных типов матриц и возможных поворотах матриц на
// углы 90, 180, 270 градусов.
CRGB* getLeds(void);

// Затемнить все светодиоды на матрице на указанный шаг
void fader(const uint8_t &step);

// Затемнить светодиод с соответствующими координатами на матрице на указанный шаг
void fadePix(const uint8_t &x, const uint8_t &y, const uint8_t &step);

// Нарисовать линию на матрице по указанным координатам и соответствующим цветом
// x1, y1 - координаты 1 точки
// x2, y2 - координаты 2 точки
// color - цвет, которым будет нарисована линия
void drawLine(const uint8_t &x1, const uint8_t &y1,
    const uint8_t &x2, const uint8_t &y2, const CRGB &color);