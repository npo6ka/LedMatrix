#pragma once

#include "effect_list/effect.h"

class Fire : public Effect
{
    static const uint8_t mask_w = 8;
    static const uint8_t mask_h = 8;

    //these values are substracetd from the generated values to give a shape to the animation
    const unsigned char valueMask[mask_h][mask_w] = {
      {32 , 0  , 0  , 0  , 0  , 0  , 0  , 32 },
      {64 , 0  , 0  , 0  , 0  , 0  , 0  , 64 },
      {96 , 32 , 0  , 0  , 0  , 0  , 32 , 96 },
      {128, 64 , 32 , 0  , 0  , 32 , 64 , 128},
      {160, 96 , 64 , 32 , 32 , 64 , 96 , 160},
      {192, 128, 96 , 64 , 64 , 96 , 128, 192},
      {255, 160, 128, 96 , 96 , 128, 160, 255},
      {255, 192, 160, 128, 128, 160, 192, 255}
    };

    //these are the hues for the fire,
    //should be between 0 (red) to about 25 (yellow)
    const unsigned char hueMask[mask_h][mask_w] = {
      {1 , 11, 19, 25, 25, 22, 11, 1},
      {1 , 8 , 13, 19, 25, 19, 8 , 1},
      {1 , 8 , 13, 16, 19, 16, 8 , 1},
      {1 , 5 , 11, 13, 13, 13, 5 , 1},
      {1 , 5 , 11, 11, 11, 11, 5 , 1},
      {0 , 1 , 5 , 8 , 8 , 5 , 1 , 0},
      {0 , 0 , 1 , 5 , 5 , 1 , 0 , 0},
      {0 , 0 , 0 , 1 , 1 , 0 , 0 , 0}
    };

    unsigned char matrixValue[mask_h][mask_w];
    unsigned char line[mask_w];
    int pcnt = 0;
    uint8_t hue_add = 0;          // добавка цвета в огонь (от 0 до 230) - меняет весь цвет пламени
    bool sparkless = true;        // вылетающие угольки вкл выкл
    bool align_center = true;     // выравнивание костра по центру

public:
    Fire() {}

    void on_init() {
        generateLine();
        memset(matrixValue, 0, sizeof(matrixValue));
    }

    void on_update() {
        if (pcnt >= 100) {
            shiftUp();
            generateLine();
            pcnt = 0;
        }

        drawFrame(pcnt);
        pcnt += 30;
    }

    // Randomly generate the next line (matrix row)
    void generateLine() {
        for (uint8_t x = 0; x < mask_w; x++) {
            line[x] = max(random8(255), random8(255));
        }
    }

    // shift all values in the matrix up one row
    void shiftUp() {
        for (uint8_t y = mask_h - 1; y > 0; y--) {
            for (uint8_t x = 0; x < mask_w; x++) {
                matrixValue[y][x] = matrixValue[y - 1][x];
            }
        }

        for (uint8_t x = 0; x < mask_w; x++) {
            matrixValue[0][x] = line[x];
        }
    }

    // draw a frame, interpolating between 2 "key frames"
    // @param pcnt percentage of interpolation
    void drawFrame(int pcnt) {
        auto bottom_y = LedMatrix.height() - 1;

        // each row interpolates with the one before it
        for (auto y : LedMatrix.rangeY()) {
            for (auto x : LedMatrix.rangeX()) {
                uint8_t tra_x = view_x_to_mask(x);
                uint8_t tra_y = bottom_y - y;
                if (tra_y < 8) {
                    int nextv = (((100.0 - pcnt) * matrixValue[tra_y][tra_x]
                        + pcnt * matrixValue[tra_y - 1][tra_x]) / 100.0)
                        - valueMask[tra_y][tra_x];

                    LedMatrix.at(x, y) = CHSV(
                       hue_add + hueMask[tra_y][tra_x], // H
                       255, // S
                       max(0, nextv) // V
                    );
                } else if (tra_y == 8 && sparkless) {
                    if (random8(20) == 0 && LedMatrix.at(x, y + 1)) {
                        LedMatrix.at(x, y) = LedMatrix.at(x, y + 1);
                    } else {
                        LedMatrix.at(x, y) = 0;
                    }
                } else if (sparkless) {
                    // старая версия для яркости
                    if (LedMatrix.at(x, y + 1)) {
                        LedMatrix.at(x, y) = LedMatrix.at(x, y + 1).fadeToBlackBy(50);
                    } else {
                        LedMatrix.at(x, y) = 0;
                    }
                }
            }
        }

        //first row interpolates with the "next" line
        for (auto x : LedMatrix.rangeX()) {
            uint8_t tra_x = view_x_to_mask(x);
            LedMatrix.at(x, bottom_y) = CHSV(
                hue_add + hueMask[0][tra_x], // H
                255,           // S
                ((100.0 - pcnt) * matrixValue[0][tra_x] + pcnt * line[tra_x]) / 100.0 // V
            );
        }
    }

    uint8_t view_x_to_mask(index_t x) {
        uint8_t res = x;
        if (align_center) {
            res += mask_w - (LedMatrix.width() - mask_w) / 2;
        }
        if (res >= mask_w) {
            res %= mask_w;
        }
        return res;
    }
};

