#include "effectslist.h"
#include "erroreffect.h"

#include "effects/slow_random.h"
#include "effects/simple_rainbow.h"
#include "effects/dribs.h"
#include "effects/rain.h"
#include "effects/all_random.h"
#include "effects/snow.h"
#include "effects/fire.h"
#include "effects/the_matrix.h"
#include "effects/simple_balls.h"
#include "effects/confetti.h"
#include "effects/starfall.h"
#include "effects/dynamic_square.h"
#include "effects/random_rain.h"
#include "effects/rainbow_rain.h"
#include "effects/points.h"
#include "effects/rainbow_point.h"
#include "effects/rainbow_static_point.h"
#include "effects/text.h"
#include "effects/mouse.h"
#include "effects/pacman.h"
#include "effects/circular_point.h"
#include "effects/zigzag.h"
#include "effects/horizontal_rainbow_point.h"
#include "effects/ny2020.h"
#include "effects/dribs_all_side.h"
#include "effects/snake/snake.h"
/*
#include "testmode.h"*/

using EffectFactory = Effect* (*)();

template <class T>
Effect *makeEffect() {
    return new T();
}

template <class T>
constexpr EffectFactory effectFactory() {
    return makeEffect<T>;
}

auto effectsFactories = {
    effectFactory<TextMode>(),
    effectFactory<Snake>(),
    effectFactory<DynamicSquare>(),
    effectFactory<TheMatrix>(),
    effectFactory<Starfall>(),
    effectFactory<RandomRain>(),
    effectFactory<Snow>(),
    effectFactory<SimpleBalls>(),
    effectFactory<Confetti>(),
    effectFactory<Rain>(),
    effectFactory<Fire>(),
    effectFactory<DribsAllSide>(),
    //effectFactory<SlowRandom>(),
    //effectFactory<SimpleRainbow>(),
    //effectFactory<Dribs>(),
    //effectFactory<AllRandom>(),
    effectFactory<RainbowRain>(),
    effectFactory<Points>(),
    //effectFactory<RainbowPoint>(),
    //effectFactory<RainbowStaticPoint>(),
    //effectFactory<Mouse>(),
    effectFactory<Pacman>(),
    effectFactory<CircularPoint>(),
    effectFactory<ZigZag>(),
    //effectFactory<HorizontalRainbowPoint>(),
    effectFactory<NY2020>(),
};

static Effect *getNewEffectInstance(const uint8_t& num) {
    if (num >= effectsFactories.size()) {
        return nullptr;
    } else {
        return effectsFactories.begin()[num]();
    }
}

// private

Effect *EffectsList::getCurEffect() const {
    return curEffect;
}

// public
EffectsList& EffectsList::getInstance() {
    static EffectsList instance;
    return instance;
}

void EffectsList::setErrorEffect() {
    clearCurEffect();
    curEffect = new ErrorEffect();
    curNum = -1;
}

uint8_t EffectsList::getCurEffectNum() const {
    return curNum;
}

void EffectsList::clearCurEffect() {
    if (getCurEffect()) {
        delete curEffect;
        curEffect = nullptr;
        curNum = -1;
    }
}

void EffectsList::setEffect(const uint8_t &num) {
    if (num > effectsFactories.size()) {
        out("ERROR: Effect number out of range\n");
        setErrorEffect();
        return;
    }

    clearCurEffect();
    curEffect = getNewEffectInstance(num);

    if (!curEffect) {
        out("ERROR: Effect not crated\n");
        setErrorEffect();
        return;
    }

    curEffect->on_clear();
    curEffect->on_init();
    curNum = num;
}

void EffectsList::nextEffect() {
    if (curNum + 1 >= (uint8_t)effectsFactories.size()) {
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
