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
#include "18_mouse.h"
#include "19_pacman.h"
#include "20_circular_point.h"
#include "21_zigzag.h"
#include "22_horizontal_rainbow_point.h"
#include "23_test_shader.h"
#include "24_ny2020.h"
/*
#include "testmode.h"*/

#define MAX_EFFECTS 50

template <class T>
Effect *effectFactory1() {
    return new T();
}

template <class T>
constexpr const auto effectFactory() {
    return effectFactory1<T>;
}

auto effectsFactories = {
    effectFactory<SlowRandom>(),            // 0
    effectFactory<SimpleRainbow>(),         // 1
    effectFactory<Dribs>(),                 // 2
    effectFactory<Rain>(),                  // 3
    effectFactory<AllRandom>(),             // 4
    effectFactory<Snow>(),                  // 5
    effectFactory<Fire>(),                  // 6
    effectFactory<TheMatrix>(),             // 7
    effectFactory<SimpleBalls>(),           // 8
    effectFactory<Confetti>(),              // 9
    effectFactory<Starfall>(),              // 10
    effectFactory<DynamicSquare>(),         // 11
    effectFactory<RandomRain>(),            // 12
    effectFactory<RainbowRain>(),           // 13
    effectFactory<Points>(),                // 14
    effectFactory<RainbowPoint>(),          // 15
    effectFactory<RainbowStaticPoint>(),    // 16
    effectFactory<TextMode>(),              // 17
    effectFactory<Mouse>(),                 // 18
    effectFactory<Pacman>(),                // 19
    effectFactory<CircularPoint>(),         // 20
    effectFactory<ZigZag>(),                // 21
    effectFactory<HorizontalRainbowPoint>(),// 22
    // effectFactory<TestShader>(),
    effectFactory<NY2020>(),                // 23
};

EffectsList& EffectsList::getInstance() {
    static EffectsList instance;
    return instance;
}

EffectsList::EffectsList() {}

Effect *EffectsList::getNewEffectInstance(uint8_t num) {
    if (num >= effectsFactories.size()) {
        return NULL;
    } else {
        return effectsFactories.begin()[num]();
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
    if (curNum + 1 >= effectsFactories.size()) {
        setEffect(0);
    } else {
        setEffect(curNum + 1);
    }
}

void EffectsList::prevEffect() {
    if (curNum - 1 < 0) {
        setEffect(effectsFactories.size() - 1);
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

        // корректировка таймера
        // если фпс меньше чем указано в режиме
        if (micros() - prev_micros > tick_size * 2) {
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
        FastLED.show();
    }
}

float EffectsList::getCurFPS() {
    return (float)fps / 10;
}
