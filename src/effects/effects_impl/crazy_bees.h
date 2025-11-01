#pragma once
// Source: https://editor.soulmatelights.com/gallery/651-crazy-bees
#include "effects/effect.h"

class Bee {
    uint8_t posX, posY, aimX, aimY, hue;
    int8_t deltaX, deltaY, signX, signY, error;

    void aimed() {
        randomSeed(millis());
        aimX = random8(0, LedMatrix.width());
        aimY = random8(0, LedMatrix.height());
        hue = random8();
        deltaX = abs(aimX - posX);
        deltaY = abs(aimY - posY);
        signX = posX < aimX ? 1 : -1;
        signY = posY < aimY ? 1 : -1;
        error = deltaX - deltaY;
    };

public:
    void on_init() {
        posX = random8(0, LedMatrix.width());
        posY = random8(0, LedMatrix.height());
        aimed();
    }


    void run() {
        LedMatrix.at(aimX + 1, aimY) += CHSV(hue, 255, 255);
        LedMatrix.at(aimX, aimY + 1) += CHSV(hue, 255, 255);
        LedMatrix.at(aimX - 1, aimY) += CHSV(hue, 255, 255);
        LedMatrix.at(aimX, aimY - 1) += CHSV(hue, 255, 255);
        if (posX != aimX || posY != aimY) {
            LedMatrix.at(posX, posY) = CHSV(hue, 60, 255);
            int8_t error2 = error * 2;
            if (error2 > -deltaY) {
                error -= deltaY;
                posX += signX;
            }
            if (error2 < deltaX) {
                error += deltaX;
                posY += signY;
            }
        } else {
            aimed();
        }
    }
};

class CrazyBees : public Effect
{
    const static uint8_t beesCnt = 1; // LEDS_SIZE / 100
    Bee bee[beesCnt];

public:
    CrazyBees() {}

    void on_init() {
        for (byte i = 0; i < beesCnt; i++) {
            bee[i].on_init();
        }
        set_fps(30);
    }

    void on_update() {
        fadeToBlackBy(LedMatrix.leds(), LedMatrix.size(), 32);
        for (byte i = 0; i < beesCnt; i++) {
            bee[i].run();
        }
        blur2d(LedMatrix.leds(), LedMatrix.width(), LedMatrix.height(), 12);
    }
};
