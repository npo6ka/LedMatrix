#pragma once
// Source: https://editor.soulmatelights.com/gallery/651-crazy-bees
#include "effects/effect.h"

class Bee {
    int16_t posX, posY, aimX, aimY;
    uint8_t hue;
    int16_t deltaX, deltaY, signX, signY, error;

    static index_t randDim(index_t lim) {
        return lim ? (index_t)random16(lim) : 0;
    }

    void aimed() {
        randomSeed(millis());
        aimX = (int16_t)randDim(LedMatrix.width());
        aimY = (int16_t)randDim(LedMatrix.height());
        hue = random8();
        deltaX = abs(aimX - posX);
        deltaY = abs(aimY - posY);
        signX = posX < aimX ? 1 : -1;
        signY = posY < aimY ? 1 : -1;
        error = deltaX - deltaY;
    };

public:
    void on_init() {
        posX = (int16_t)randDim(LedMatrix.width());
        posY = (int16_t)randDim(LedMatrix.height());
        aimed();
    }


    void run() {
        const index_t w = LedMatrix.width();
        const index_t h = LedMatrix.height();
        if (aimX + 1 < (int16_t)w) LedMatrix.at(aimX + 1, aimY) += CHSV(hue, 255, 255);
        if (aimY + 1 < (int16_t)h) LedMatrix.at(aimX, aimY + 1) += CHSV(hue, 255, 255);
        if (aimX > 0) LedMatrix.at(aimX - 1, aimY) += CHSV(hue, 255, 255);
        if (aimY > 0) LedMatrix.at(aimX, aimY - 1) += CHSV(hue, 255, 255);
        if (posX != aimX || posY != aimY) {
            LedMatrix.at(posX, posY) = CHSV(hue, 60, 255);
            int16_t error2 = error * 2;
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
        LedMatrix.blur(12);
    }
};
