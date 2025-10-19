#pragma once

#include <stdint.h>
#include <Arduino.h>

class FpsManager {
    public:
        FpsManager() = default;
        ~FpsManager() = default;

        float getRealFPS() const {
            return (float)_real_fps / 10;
        }

        void setTargetFPS(uint16_t fps) {
            _target_fps = fps;
        }

        bool needUpdate() {
            bool need = false;
            unsigned long tick_size = 1000000 / _target_fps;

            // проверяем соответствие фпса указанному в режиме
            if (micros() - _prev_micros > tick_size) {
                // корректировка таймера
                // если фпс меньше чем указано в режиме
                if (micros() - _prev_micros > tick_size * 2) {
                    // считаем реальный фпс
                    _real_fps = (_real_fps + 1000000 * 10 / (micros() - _prev_micros - tick_size)) / 2;
                    // следующей итерацией обязательно вызываем on_update + корректируем время
                    // чтобы prev_micros сильно не уходило от текущего времени
                    _prev_micros = micros() - tick_size;
                } else {
                    // иначе просто прибавляем размер тика в микросекундах
                    _prev_micros += tick_size;
                    // считаем реальный фпс
                    _real_fps = (_real_fps + _target_fps * 10) / 2;
                }
                need = true;
            }
            return need;
        }

    private:
        unsigned long _prev_micros;
        unsigned long _real_fps;
        uint16_t _target_fps = 30;
};
