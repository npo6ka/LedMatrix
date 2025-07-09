#include "effect_factory.h"

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
#include "effects/radial_fire.h"
#include "effects/radial_pattern.h"
#include "effects/crazy_bees.h"

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
#define EFFECT_COUNT 28

static EffectInfo getEffectInfo(int effect_id) {
    switch (effect_id) {
        EFFECT_CASE(0, "SlowRandom", SlowRandom)
        EFFECT_CASE(1, "SimpleRainbow", SimpleRainbow)
        EFFECT_CASE(2, "Dribs", Dribs)
        EFFECT_CASE(3, "Rain", Rain)
        EFFECT_CASE(4, "AllRandom", AllRandom)
        EFFECT_CASE(5, "Snow", Snow)
        EFFECT_CASE(6, "Fire", Fire)
        EFFECT_CASE(7, "TheMatrix", TheMatrix)
        EFFECT_CASE(8, "SimpleBalls", SimpleBalls)
        EFFECT_CASE(9, "Confetti", Confetti)
        EFFECT_CASE(10, "Starfall", Starfall)
        EFFECT_CASE(11, "DynamicSquare", DynamicSquare)
        EFFECT_CASE(12, "RandomRain", RandomRain)
        EFFECT_CASE(13, "RainbowRain", RainbowRain)
        EFFECT_CASE(14, "Points", Points)
        EFFECT_CASE(15, "RainbowPoint", RainbowPoint)
        EFFECT_CASE(16, "RainbowStaticPoint", RainbowStaticPoint)
        EFFECT_CASE(17, "Text", TextMode)
        EFFECT_CASE(18, "Mouse", Mouse)
        EFFECT_CASE(19, "Pacman", Pacman)
        EFFECT_CASE(20, "CircularPoint", CircularPoint)
        EFFECT_CASE(21, "Zigzag", ZigZag)
        EFFECT_CASE(22, "HorizontalRainbowPoint", HorizontalRainbowPoint)
        EFFECT_CASE(23, "Ny2020", NY2020)
        EFFECT_CASE(24, "DribsAllSide", DribsAllSide)
        EFFECT_CASE(25, "Snake", Snake)
        EFFECT_CASE(26, "RadialFire", RadialFire)
        EFFECT_CASE(27, "RadialPattern", RadialPattern)
        EFFECT_CASE(28, "CrazyBees", CrazyBees)
        // при добавлении нового эффекта, не забудь обновить EFFECT_COUNT

        default:
            return EffectInfo{"Error", effectCreator<ErrorEffect>()};
    }
}

uint16_t EffectFactory::getEffectCount() {
    return EFFECT_COUNT;
}

Effect* EffectFactory::createEffect(int effect_id) {
    return getEffectInfo(effect_id).effect_creator();
}

const char* EffectFactory::getEffectName(int effect_id) {
    return getEffectInfo(effect_id).effect_name;
}
