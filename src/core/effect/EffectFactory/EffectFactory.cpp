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
#include "effects/effects_impl/spiral.h"
#include "effects/effects_impl/pulse_rings.h"
#include "effects/effects_impl/MusicSpectrum.h"
#include "effects/effects_impl/static_color.h"
#include "effects/effects_impl/color_blink.h"
#include "effects/effects_impl/blend_wave.h"

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
    InputCapability required;
};

#define EFFECT_CASE(type) \
    case static_cast<uint32_t>(EffectId::type): \
        return EffectCreationInfo{#type, effectCreator<type>(), type::kRequired};

static EffectCreationInfo getEffectInfo(uint32_t effect_id) {
    switch (effect_id) {
        EFFECT_CASE(ErrorEffect);
        EFFECT_CASE(SlowRandom);
        EFFECT_CASE(SimpleRainbow);
        EFFECT_CASE(Dribs);
        EFFECT_CASE(Rain);
        EFFECT_CASE(AllRandom);
        EFFECT_CASE(Snow);
        EFFECT_CASE(Fire);
        EFFECT_CASE(TheMatrix);
        EFFECT_CASE(SimpleBalls);
        EFFECT_CASE(Confetti);
        EFFECT_CASE(Starfall);
        EFFECT_CASE(DynamicSquare);
        EFFECT_CASE(RandomRain);
        EFFECT_CASE(RainbowRain);
        EFFECT_CASE(Points);
        EFFECT_CASE(RainbowPoint);
        EFFECT_CASE(RainbowStaticPoint);
        EFFECT_CASE(TextMode);
        EFFECT_CASE(Mouse);
        EFFECT_CASE(Pacman);
        EFFECT_CASE(CircularPoint);
        EFFECT_CASE(ZigZag);
        EFFECT_CASE(HorizontalRainbowPoint);
        EFFECT_CASE(NY2020);
        EFFECT_CASE(DribsAllSide);
        EFFECT_CASE(Snake);
        EFFECT_CASE(RadialFire);
        EFFECT_CASE(RadialPattern);
        EFFECT_CASE(CrazyBees);
        EFFECT_CASE(Spiral);
        EFFECT_CASE(PulseRings);
        EFFECT_CASE(MusicSpectrum);
        EFFECT_CASE(StaticColor);
        EFFECT_CASE(ColorBlink);
        EFFECT_CASE(BlendWave);

        default:
            return EffectCreationInfo{"ErrorEffect", effectCreator<ErrorEffect>(), Effect::kRequired};
    }
}

uint32_t EffectFactory::getEffectCount() {
    return static_cast<uint32_t>(EffectId::Count);
}

std::unique_ptr<Effect> EffectFactory::createEffect(uint32_t effect_id) {
    return getEffectInfo(effect_id).effect_creator();
}

const char* EffectFactory::getEffectName(uint32_t effect_id) {
    return getEffectInfo(effect_id).effect_name;
}

InputCapability EffectFactory::getRequiredCapabilities(uint32_t effect_id) {
    return getEffectInfo(effect_id).required;
}

#undef EFFECT_CASE