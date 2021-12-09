#pragma once

#include "effect.h"

class SimpleBalls : public Effect
{
    static const uint8_t balls_amount = 3;
    int coord[balls_amount][2];
    int8_t vector[balls_amount][2];
    CRGB ballColors[balls_amount];
    bool ball_track = true;     // (0 / 1) - вкл/выкл следы шариков
    uint8_t track_step = 70;    // длина хвоста шарика (чем больше цифра, тем хвост короче)
    bool draw_walls = false;    // режим с рисованием препятствий для шаров
    uint32_t wall_color = 0x00ff00;

public:
    SimpleBalls() {}

    void on_init() {
        for (uint8_t j = 0; j < balls_amount; j++) {
            int sign;

            // забиваем случайными данными
            coord[j][0] = HEIGHT / 2 * 10;
            random(2) ? sign = 1 : sign = -1;
            vector[j][0] = random(4, 15) * sign;
            coord[j][1] = WIDTH / 2 * 10;
            random(2) ? sign = 1 : sign = -1;
            vector[j][1] = random(4, 15) * sign;
            ballColors[j] = CHSV(random(0, 9) * 28, 255, 255);
        }
        set_fps(30);
    }

    void on_update() {
        if (!ball_track)    // если режим БЕЗ следов шариков
            FastLED.clear();  // очистить
        else {              // режим со следами
            fader(track_step);
        }

        getLeds()[0].fadeToBlackBy(1);

        // движение шариков
        for (uint8_t j = 0; j < balls_amount; j++) {
            // отскок от нарисованных препятствий
            if (draw_walls) {
                uint32_t thisColor = getPixColor(coord[j][0] / 10 + 1, coord[j][1] / 10);
                if (thisColor == wall_color/* && vector[j][0] > 0*/) {
                    vector[j][0] = -vector[j][0];
                }
                thisColor = getPixColor(coord[j][0] / 10 - 1, coord[j][1] / 10);
                if (thisColor == wall_color/* && vector[j][0] < 0*/) {
                    vector[j][0] = -vector[j][0];
                }
                thisColor = getPixColor(coord[j][0] / 10, coord[j][1] / 10 + 1);
                if (thisColor == wall_color/* && vector[j][1] > 0*/) {
                    vector[j][1] = -vector[j][1];
                }
                thisColor = getPixColor(coord[j][0] / 10, coord[j][1] / 10 - 1);
                if (thisColor == wall_color/* && vector[j][1] < 0*/) {
                    vector[j][1] = -vector[j][1];
                }
            }

            // движение шариков
            for (uint8_t i = 0; i < 2; i++) {
                coord[j][i] += vector[j][i];
                if (coord[j][i] < 0) {
                    coord[j][i] = 0;
                    vector[j][i] = -vector[j][i];
                }
            }

            if (coord[j][0] > (HEIGHT - 1) * 10) {
                coord[j][0] = (HEIGHT - 1) * 10;
                vector[j][0] = -vector[j][0];
            }
            if (coord[j][1] > (WIDTH - 1) * 10) {
                coord[j][1] = (WIDTH - 1) * 10;
                vector[j][1] = -vector[j][1];
            }
            setPixColor(coord[j][0] / 10, coord[j][1] / 10, ballColors[j]);
        }
    }
};
