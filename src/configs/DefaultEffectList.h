#pragma once

#include "core/effect/EffectFactory.h"

#include <cstddef>

// Список id эффектов по умолчанию (начальный список при инициализации / сбросе памяти).
// Можно удалять, добавлять, менять порядок, дублировать эффекты — размер kIds выводится автоматически.
//
// Данные constexpr обычно лежат во flash (.rodata), а не в оперативной памяти; в RAM — только копия
// в векторе хранилища эффектов во время работы.
namespace DefaultEffects {
    static constexpr EffectId effectIds[] = {
        EffectId::SlowRandom,
        EffectId::SimpleRainbow,
        EffectId::Dribs,
        EffectId::Rain,
        EffectId::AllRandom,
        EffectId::Snow,
        EffectId::Fire,
        EffectId::TheMatrix,
        EffectId::SimpleBalls,
        EffectId::Confetti,
        EffectId::Starfall,
        EffectId::DynamicSquare,
        EffectId::RandomRain,
        EffectId::RainbowRain,
        EffectId::Points,
        EffectId::RainbowPoint,
        EffectId::RainbowStaticPoint,
        EffectId::TextMode,
        EffectId::Mouse,
        EffectId::Pacman,
        EffectId::CircularPoint,
        EffectId::ZigZag,
        EffectId::HorizontalRainbowPoint,
        EffectId::NY2020,
        EffectId::DribsAllSide,
        EffectId::Snake,
        EffectId::RadialFire,
        EffectId::RadialPattern,
        EffectId::CrazyBees,
        EffectId::Spiral,
        EffectId::PulseRings,
    };
} // namespace DefaultEffects
