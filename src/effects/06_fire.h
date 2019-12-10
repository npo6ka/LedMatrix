#pragma once

#include "effect.h"

class Fire : public Effect
{
    //these values are substracetd from the generated values to give a shape to the animation
    const unsigned char valueMask[8][16] PROGMEM = {
      {32 , 0  , 0  , 0  , 0  , 0  , 0  , 32 , 32 , 0  , 0  , 0  , 0  , 0  , 0  , 32 },
      {64 , 0  , 0  , 0  , 0  , 0  , 0  , 64 , 64 , 0  , 0  , 0  , 0  , 0  , 0  , 64 },
      {96 , 32 , 0  , 0  , 0  , 0  , 32 , 96 , 96 , 32 , 0  , 0  , 0  , 0  , 32 , 96 },
      {128, 64 , 32 , 0  , 0  , 32 , 64 , 128, 128, 64 , 32 , 0  , 0  , 32 , 64 , 128},
      {160, 96 , 64 , 32 , 32 , 64 , 96 , 160, 160, 96 , 64 , 32 , 32 , 64 , 96 , 160},
      {192, 128, 96 , 64 , 64 , 96 , 128, 192, 192, 128, 96 , 64 , 64 , 96 , 128, 192},
      {255, 160, 128, 96 , 96 , 128, 160, 255, 255, 160, 128, 96 , 96 , 128, 160, 255},
      {255, 192, 160, 128, 128, 160, 192, 255, 255, 192, 160, 128, 128, 160, 192, 255}
    };

    //these are the hues for the fire,
    //should be between 0 (red) to about 25 (yellow)
    const unsigned char hueMask[8][16] PROGMEM = {
      {1 , 11, 19, 25, 25, 22, 11, 1 , 1 , 11, 19, 25, 25, 22, 11, 1 },
      {1 , 8 , 13, 19, 25, 19, 8 , 1 , 1 , 8 , 13, 19, 25, 19, 8 , 1 },
      {1 , 8 , 13, 16, 19, 16, 8 , 1 , 1 , 8 , 13, 16, 19, 16, 8 , 1 },
      {1 , 5 , 11, 13, 13, 13, 5 , 1 , 1 , 5 , 11, 13, 13, 13, 5 , 1 },
      {1 , 5 , 11, 11, 11, 11, 5 , 1 , 1 , 5 , 11, 11, 11, 11, 5 , 1 },
      {0 , 1 , 5 , 8 , 8 , 5 , 1 , 0 , 0 , 1 , 5 , 8 , 8 , 5 , 1 , 0 },
      {0 , 0 , 1 , 5 , 5 , 1 , 0 , 0 , 0 , 0 , 1 , 5 , 5 , 1 , 0 , 0 },
      {0 , 0 , 0 , 1 , 1 , 0 , 0 , 0 , 0 , 0 , 0 , 1 , 1 , 0 , 0 , 0 }
    };

    unsigned char matrixValue[8][16];
    unsigned char line[WIDTH];
    int pcnt = 0;
    uint8_t hue_add = 0;       // добавка цвета в огонь (от 0 до 230) - меняет весь цвет пламени
    bool sparkless = true;        // вылетающие угольки вкл выкл

public:
    Fire() {}

    void on_init() {
        generateLine();
        memset(matrixValue, 0, sizeof(matrixValue));
    }

    void on_tick() {
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
        for (uint8_t x = 0; x < WIDTH; x++) {
            line[x] = max(random8(255), random8(255));
        }
    }

    //shift all values in the matrix up one row

    void shiftUp() {
        for (uint8_t y = HEIGHT - 1; y > 0; y--) {
            for (uint8_t x = 0; x < WIDTH; x++) {
                uint8_t newX = x;
                if (x > 15) newX = x - 15;
                if (y > 7) continue;
                matrixValue[y][newX] = matrixValue[y - 1][newX];
            }
        }

        for (uint8_t x = 0; x < WIDTH; x++) {
            uint8_t newX = x;
            if (x > 15) {
                newX = x - 15;
            }
            matrixValue[0][newX] = line[newX];
        }
    }

    // draw a frame, interpolating between 2 "key frames"
    // @param pcnt percentage of interpolation

    void drawFrame(int pcnt) {
        int nextv;

        //each row interpolates with the one before it
        for (uint8_t y = HEIGHT - 1; y > 0; y--) {
            for (uint8_t x = 0; x < WIDTH; x++) {
                uint8_t newX = x;
                if (x > 15) {
                    newX = x - 15;
                }
                if (y < 8) {
                    uint8_t val = pgm_read_byte(&(valueMask[y][newX]));
                    nextv = (((100.0 - pcnt) * matrixValue[y][newX]
                        + pcnt * matrixValue[y - 1][newX]) / 100.0)
                        - pgm_read_byte(&(valueMask[y][newX]));

                    CRGB color = CHSV(
                       hue_add + pgm_read_byte(&(hueMask[y][newX])), // H
                       255, // S
                       (uint8_t)max(0, nextv) // V
                    );

                    setPixColor(x, y, color);
                } else if (y == 8 && sparkless) {
                    if (random8(20) == 0 && getPixColor(x, y - 1) != 0) {
                        setPixColor(x, y, getPixColor(x, y - 1));
                    } else {
                        setPixColor(x, y, 0);
                    }
                } else if (sparkless) {
                    // старая версия для яркости
                    if (getPixColor(x, y - 1) > 0) {
                        setPixColor(x, y, getPixColor(x, y - 1));
                    } else {
                        setPixColor(x, y, 0);
                    }
                }
            }
        }

        //first row interpolates with the "next" line
        for (unsigned char x = 0; x < WIDTH; x++) {
            uint8_t newX = x;
            if (x > 15) {
                newX = x - 15;
            }
            CRGB color = CHSV(
                hue_add + pgm_read_byte(&(hueMask[0][newX])), // H
                255,           // S
                (uint8_t)(((100.0 - pcnt) * matrixValue[0][newX] + pcnt * line[newX]) / 100.0) // V
            );

            setPixColor(newX, 0, color);
        }
    }
};

