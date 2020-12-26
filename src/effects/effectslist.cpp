#include "effectslist.h"

#include "00_slow_random.h"
#include "01_simple_rainbow.h"
#include "02_dribs.h"
#include "03_rain.h"
#include "04_all_random.h"
#include "05_snow.h"
#include "06_fire.h"
#include "07_the_matrix.h"
#include "08_simple_balls.h"
#include "09_confetti.h"
#include "10_starfall.h"
#include "11_dynamic_square.h"
#include "12_random_rain.h"
#include "13_rainbow_rain.h"
#include "14_points.h"
#include "15_rainbow_point.h"
#include "16_rainbow_static_point.h"
#include "17_text.h"
#include "_18_Snake.h"
#include "_19_Empty.h"
#include "_20_TestEffect.h"
#include "18_mouse.h"
#include "19_pacman.h"
#include "20_circular_point.h"
#include "21_zigzag.h"
#include "22_horizontal_rainbow_point.h"
#include "23_test_shader.h"
#include "24_ny2020.h"
#include "_20_TestEffect.h"
/*
#include "testmode.h"*/

#define MAX_EFFECTS 50

EffectsList& EffectsList::getInstance() {
    static EffectsList instance;
    return instance;
}

EffectsList::EffectsList() {
    init();
}

void EffectsList::init() {
    amnt = MAX_EFFECTS;
    Effect *eff = NULL;
    curEffect = nullptr;

    while(eff == NULL && amnt >= 0) {
        amnt--;
        eff = getNewEffectInstance(amnt);
    }

    if (eff == NULL) {
        setErrorEffect();
    } else {
        curNum = amnt;
        setEffect(eff);
        amnt += 1;
    }
}

Effect *EffectsList::getNewEffectInstance(int num) {
    switch (num) {

    case 0:
        return new Rain();
    case 1:
        return new Dribs();
    case 2:
        return new Snow();
    case 3:
        return new Fire();
    case 4:
        return new TheMatrix();
    case 5:
        return new SimpleBalls();
    case 6:
        return new Confetti();
    case 7:
        return new Starfall();
    case 8:
        return new DynamicSquare();
    case 9:
        return new Points();
    case 10:
        return new RainbowPoint();
    case 11:
        return new TextMode();
    case 12:
        return new Pacman();
    case 13:
        return new CircularPoint();
    case 14:
        return new ZigZag();
    case 15:
        return new NY2020();
    case 16:
        return new Snake();

    // case 0:
    //     return new RandomRain();
    // case 1:
    //     return new Rain();
    // case 2:
    //     return new Confetti();
    // case 3:
    //     return new SimpleBalls();
    // case 4:
    //     return new Snow();
    // case 5:
    //     return new Dribs();
    // case 6:
    //     return new ZigZag();
    default:
        return NULL;
    }
}

void EffectsList::setErrorEffect() {
    clearCurEffect();
    curEffect = new ErrorEffect();
}

Effect *EffectsList::getCurEffect() {
    return curEffect;
}

int EffectsList::getCurEffectNum() {
    return curNum;
}

void EffectsList::clearCurEffect() {
    if (getCurEffect()) {
        delete curEffect;
        curEffect = nullptr;
    }
}

void EffectsList::setEffect(int num) {
    setEffect(getNewEffectInstance(num));
    curNum = num;
}

void EffectsList::setEffect(Effect *eff) {
    clearCurEffect();

    curEffect = eff;
    if (curEffect == NULL) {
        setErrorEffect();
        curNum = -1;
        return;
    }

    curEffect->on_clear();
    curEffect->on_init();
}

void EffectsList::nextEffect() {
    if (curNum + 1 >= amnt) {
        setEffect(0);
    } else {
        setEffect(curNum + 1);
    }
}

void EffectsList::prevEffect() {
    if (curNum - 1 < 0) {
        setEffect(amnt - 1);
    } else {
        setEffect(curNum - 1);
    }
}

//перезапустить текущий эффект
void EffectsList::reloadCurEff() {
    setEffect(curNum);
}

void EffectsList::onTick() {
    unsigned long tick_size = 1000000 / curEffect->get_fps();

    // проверяем соответствие фпса указанному в режиме
    if (micros() - prev_micros > tick_size) {
        curEffect->on_update();
        curEffect->on_render();
        FastLED.show();

        // корректировка таймера
        // если фпс меньше чем указано в режиме
        if (abs(micros() - prev_micros) > tick_size * 2) {
            // считаем реальный фпс
            fps = (fps + 1000000 * 10 / (micros() - prev_micros - tick_size)) / 2;
            // следующей итерацией обязательно вызываем on_update + корректируем время
            // чтобы prev_micros сильно не уходило от текущего времени
            prev_micros = micros() - tick_size;
        } else {
            // иначе просто прибавляем размер тика в микросекундах
            prev_micros += tick_size;
            // считаем реальный фпс
            fps = (fps + curEffect->get_fps() * 10) / 2;
        }       
    }
}

float EffectsList::getCurFPS() {
    return (float)fps / 10;
}

void EffectsList::setNadysText() {
    setEffect(new TextMode());
}

void EffectsList::setEmptyMode() {
    setEffect(new Empty());
}
