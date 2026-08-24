#pragma once

#include "core/effect/EffectFactory/EffectId.h"

// Список id эффектов по умолчанию (начальный список при инициализации / сбросе памяти).
// Можно удалять, добавлять, менять порядок, дублировать эффекты — размер kIds выводится автоматически.

namespace DefaultEffects {
    static constexpr EffectId effectIds[] = {
        EffectId::SlowRandom,
        EffectId::SimpleRainbow,
        EffectId::Dribs,
        //EffectId::Rain,
        EffectId::AllRandom,
        //EffectId::Snow,
        EffectId::Fire,
        //EffectId::TheMatrix,
        EffectId::SimpleBalls,
        EffectId::Confetti,
        //EffectId::Starfall,
        //EffectId::DynamicSquare,
        //EffectId::RandomRain,
        EffectId::RainbowRain,
        EffectId::Points,
        EffectId::RainbowPoint,
        EffectId::RainbowStaticPoint,
        //EffectId::TextMode,
        //EffectId::Mouse,
        //EffectId::Pacman,
        EffectId::CircularPoint,
        //EffectId::ZigZag,
        //EffectId::HorizontalRainbowPoint,
        //EffectId::NY2020,
        EffectId::DribsAllSide,
        EffectId::Snake,
        EffectId::RadialFire,
        EffectId::RadialPattern,
        EffectId::CrazyBees,
        EffectId::Spiral,
        //EffectId::PulseRings,
        EffectId::MusicSpectrum,
        EffectId::StaticColor,
        EffectId::ColorBlink,
        EffectId::BlendWave,
        EffectId::RainbowBeat,
        EffectId::TwoWaves,
        EffectId::OneWave,
        EffectId::Noise8,
        EffectId::Plasma,
        EffectId::Juggle,
        EffectId::MusicRunner,
        EffectId::MusicComet,
        EffectId::MusicSlider,
    };
} // namespace DefaultEffects
