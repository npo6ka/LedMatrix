#include "EffectFactory.h"

#include "effects/erroreffect.h"
#include "effects/effects_impl/slow_random.h"
#include "effects/effects_impl/simple_rainbow.h"
#include "effects/effects_impl/dribs.h"
#include "effects/effects_impl/rain.h"
#include "effects/effects_impl/all_random.h"
#include "effects/effects_impl/snow.h"
#include "effects/effects_impl/fire.h"
#include "effects/effects_impl/the_matrix.h"
#include "effects/effects_impl/simple_balls.h"
#include "effects/effects_impl/confetti.h"
#include "effects/effects_impl/starfall.h"
#include "effects/effects_impl/dynamic_square.h"
#include "effects/effects_impl/random_rain.h"
#include "effects/effects_impl/rainbow_rain.h"
#include "effects/effects_impl/points.h"
#include "effects/effects_impl/rainbow_point.h"
#include "effects/effects_impl/rainbow_static_point.h"
#include "effects/effects_impl/text.h"
#include "effects/effects_impl/mouse.h"
#include "effects/effects_impl/pacman.h"
#include "effects/effects_impl/circular_point.h"
#include "effects/effects_impl/zigzag.h"
#include "effects/effects_impl/horizontal_rainbow_point.h"
#include "effects/effects_impl/ny2020.h"
#include "effects/effects_impl/dribs_all_side.h"
#include "effects/effects_impl/snake/snake.h"
#include "effects/effects_impl/radial_fire.h"
#include "effects/effects_impl/radial_pattern.h"
#include "effects/effects_impl/crazy_bees.h"

#include "libs/StdFeatures.h"

using EffectCreator = std::unique_ptr<Effect> (*)();

template <class T>
static std::unique_ptr<Effect> makeEffect() {
    return std::make_unique<T>();
}

template <class T>
static constexpr EffectCreator effectCreator() {
    return makeEffect<T>;
}

struct EffectCreationInfo {
    const char* effect_name;
    EffectCreator effect_creator;
};

#define EFFECT_CASE(id, name, type) case id: return EffectCreationInfo{name, effectCreator<type>()};
#define EFFECT_COUNT 19 // при добавлении нового эффекта, не забудь обновить EFFECT_COUNT

static EffectCreationInfo getEffectInfo(uint32_t effect_id) {
    switch (effect_id) {
        EFFECT_CASE(0, "Error", ErrorEffect);
        //EFFECT_CASE(1, "SlowRandom", SlowRandom);
        //EFFECT_CASE(2, "SimpleRainbow", SimpleRainbow);
        //EFFECT_CASE(3, "Dribs", Dribs);
        EFFECT_CASE(1, "Rain", Rain);
        //EFFECT_CASE(5, "AllRandom", AllRandom);
        //EFFECT_CASE(6, "Snow", Snow);
        //EFFECT_CASE(7, "Fire", Fire);
        EFFECT_CASE(2, "TheMatrix", TheMatrix);
        EFFECT_CASE(3, "SimpleBalls", SimpleBalls);
        EFFECT_CASE(4, "Confetti", Confetti);
        //EFFECT_CASE(11, "Starfall", Starfall);
        //EFFECT_CASE(12, "DynamicSquare", DynamicSquare);
        EFFECT_CASE(5, "RandomRain", RandomRain);
        EFFECT_CASE(6, "RainbowRain", RainbowRain);
        EFFECT_CASE(7, "Points", Points);
        EFFECT_CASE(8, "RainbowPoint", RainbowPoint);
        //EFFECT_CASE(17, "RainbowStaticPoint", RainbowStaticPoint);
        EFFECT_CASE(9, "Text", TextMode);
        EFFECT_CASE(10, "Mouse", Mouse);
        EFFECT_CASE(11, "Pacman", Pacman);
        EFFECT_CASE(12, "CircularPoint", CircularPoint);
        EFFECT_CASE(13, "Zigzag", ZigZag);
        //EFFECT_CASE(23, "HorizontalRainbowPoint", HorizontalRainbowPoint);
        EFFECT_CASE(14, "Ny2020", NY2020);
        EFFECT_CASE(15, "DribsAllSide", DribsAllSide);
        EFFECT_CASE(16, "Snake", Snake);
        EFFECT_CASE(17, "RadialFire", RadialFire);
        EFFECT_CASE(18, "RadialPattern", RadialPattern);
        EFFECT_CASE(19, "CrazyBees", CrazyBees);
        // при добавлении нового эффекта, не забудь обновить EFFECT_COUNT

        default:
            return EffectCreationInfo{"Error", effectCreator<ErrorEffect>()};
    }
}

uint32_t EffectFactory::getEffectCount() {
    return EFFECT_COUNT;
}

std::unique_ptr<Effect> EffectFactory::createEffect(uint32_t effect_id) {
    return getEffectInfo(effect_id).effect_creator();
}

const char* EffectFactory::getEffectName(uint32_t effect_id) {
    return getEffectInfo(effect_id).effect_name;
}
