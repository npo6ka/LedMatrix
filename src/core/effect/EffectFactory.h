#pragma once

#include "effects/effect.h"

#include <stdint.h>
#include <memory>

enum class EffectId : uint32_t {
    ErrorEffect = 0,
    SlowRandom = 1,
    SimpleRainbow = 2,
    Dribs = 3,
    Rain = 4,
    AllRandom = 5,
    Snow = 6,
    Fire = 7,
    TheMatrix = 8,
    SimpleBalls = 9,
    Confetti = 10,
    Starfall = 11,
    DynamicSquare = 12,
    RandomRain = 13,
    RainbowRain = 14,
    Points = 15,
    RainbowPoint = 16,
    RainbowStaticPoint = 17,
    TextMode = 18,
    Mouse = 19,
    Pacman = 20,
    CircularPoint = 21,
    ZigZag = 22,
    HorizontalRainbowPoint = 23,
    NY2020 = 24,
    DribsAllSide = 25,
    Snake = 26,
    RadialFire = 27,
    RadialPattern = 28,
    CrazyBees = 29,
    Spiral = 30,
    PulseRings = 31,
    Count
};

class EffectFactory {
public:
    static std::unique_ptr<Effect> createEffect(uint32_t effect_id);
    static const char* getEffectName(uint32_t effect_id);
    static uint32_t getEffectCount();
};


