#pragma once

#include "stddef.h"

void debugSetup();

/*
  * Реализует функциональность printf в Serial.print
  * Применяется для отладочной печати
  * Параметры как у printf
  * Возвращает
  *    0 - ошибка формата
  *    отрицательное число - нехватка памяти, модуль числа равен запрашиваемой памяти
  *    положительное число - количество символов, выведенное в Serial
  */
const size_t out(const char *szFormat, ...);

#define logInfo(fmt, ...) out("[INFO] " fmt, ##__VA_ARGS__)
#define logError(fmt, ...) out("[ERROR] " fmt, ##__VA_ARGS__)
