#include "EffectFactory.h"

#include "effect_list/erroreffect.h"
#include "effect_list/effects/slow_random.h"
#include "effect_list/effects/simple_rainbow.h"
#include "effect_list/effects/dribs.h"
#include "effect_list/effects/rain.h"
#include "effect_list/effects/all_random.h"
#include "effect_list/effects/snow.h"
#include "effect_list/effects/fire.h"
#include "effect_list/effects/the_matrix.h"
#include "effect_list/effects/simple_balls.h"
#include "effect_list/effects/confetti.h"
#include "effect_list/effects/starfall.h"
#include "effect_list/effects/dynamic_square.h"
#include "effect_list/effects/random_rain.h"
#include "effect_list/effects/rainbow_rain.h"
#include "effect_list/effects/points.h"
#include "effect_list/effects/rainbow_point.h"
#include "effect_list/effects/rainbow_static_point.h"
#include "effect_list/effects/text.h"
#include "effect_list/effects/mouse.h"
#include "effect_list/effects/pacman.h"
#include "effect_list/effects/circular_point.h"
#include "effect_list/effects/zigzag.h"
#include "effect_list/effects/horizontal_rainbow_point.h"
#include "effect_list/effects/ny2020.h"
#include "effect_list/effects/dribs_all_side.h"
#include "effect_list/effects/snake/snake.h"
#include "effect_list/effects/radial_fire.h"
#include "effect_list/effects/radial_pattern.h"
#include "effect_list/effects/crazy_bees.h"

using EffectCreator = Effect* (*)();

template <class T>
static Effect *makeEffect() {
    return new T();
}

template <class T>
static constexpr EffectCreator effectCreator() {
    return makeEffect<T>;
}

struct EffectInfo {
    const char* effect_name;
    EffectCreator effect_creator;
};

#define EFFECT_CASE(id, name, type) case id: return EffectInfo{name, effectCreator<type>()};
#define EFFECT_COUNT 29 // при добавлении нового эффекта, не забудь обновить EFFECT_COUNT

static EffectInfo getEffectInfo(uint32_t effect_id) {
    switch (effect_id) {
        EFFECT_CASE(0, "Error", ErrorEffect);
        EFFECT_CASE(1, "SlowRandom", SlowRandom);
        EFFECT_CASE(2, "SimpleRainbow", SimpleRainbow);
        EFFECT_CASE(3, "Dribs", Dribs);
        EFFECT_CASE(4, "Rain", Rain);
        EFFECT_CASE(5, "AllRandom", AllRandom);
        EFFECT_CASE(6, "Snow", Snow);
        EFFECT_CASE(7, "Fire", Fire);
        EFFECT_CASE(8, "TheMatrix", TheMatrix);
        EFFECT_CASE(9, "SimpleBalls", SimpleBalls);
        EFFECT_CASE(10, "Confetti", Confetti);
        EFFECT_CASE(11, "Starfall", Starfall);
        EFFECT_CASE(12, "DynamicSquare", DynamicSquare);
        EFFECT_CASE(13, "RandomRain", RandomRain);
        EFFECT_CASE(14, "RainbowRain", RainbowRain);
        EFFECT_CASE(15, "Points", Points);
        EFFECT_CASE(16, "RainbowPoint", RainbowPoint);
        EFFECT_CASE(17, "RainbowStaticPoint", RainbowStaticPoint);
        EFFECT_CASE(18, "Text", TextMode);
        EFFECT_CASE(19, "Mouse", Mouse);
        EFFECT_CASE(20, "Pacman", Pacman);
        EFFECT_CASE(21, "CircularPoint", CircularPoint);
        EFFECT_CASE(22, "Zigzag", ZigZag);
        EFFECT_CASE(23, "HorizontalRainbowPoint", HorizontalRainbowPoint);
        EFFECT_CASE(24, "Ny2020", NY2020);
        EFFECT_CASE(25, "DribsAllSide", DribsAllSide);
        EFFECT_CASE(26, "Snake", Snake);
        EFFECT_CASE(27, "RadialFire", RadialFire);
        EFFECT_CASE(28, "RadialPattern", RadialPattern);
        EFFECT_CASE(29, "CrazyBees", CrazyBees);
        // при добавлении нового эффекта, не забудь обновить EFFECT_COUNT

        default:
            return EffectInfo{"Error", effectCreator<ErrorEffect>()};
    }
}

uint32_t EffectFactory::getEffectCount() {
    return EFFECT_COUNT;
}

Effect* EffectFactory::createEffect(uint32_t effect_id) {
    return getEffectInfo(effect_id).effect_creator();
}

const char* EffectFactory::getEffectName(uint32_t effect_id) {
    return getEffectInfo(effect_id).effect_name;
}
