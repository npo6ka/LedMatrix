# pragma once

#include "core/effect/EffectInfo.h"
#include "IEffectStorage.h"

#include "vector"

class StaticEffectStorage : public IEffectStorage
{
private:
    std::vector<EffectInfo> _effects;
    uint32_t _currentEffectIndex = 0;
public:
    StaticEffectStorage() {
        createDefaultEffectsList();
    }

    virtual ~StaticEffectStorage() {};

    virtual EffectInfo getEffectInfo(uint32_t index) const override{
        if (index >= _effects.size()) {
            return EffectInfo();
        }
        return _effects[index];
    }

    virtual uint32_t getCurrentIndex() const override{
        return _currentEffectIndex;
    }

    virtual void setCurrentIndex(uint32_t index) override{
        if (index >= _effects.size()) {
            return;
        }
        _currentEffectIndex = index;
    }

    virtual void addEffect(uint32_t effectId) override {
        internalAddEffect(effectId);
    }

    virtual void addEffect(uint32_t effectId, uint32_t position) override{
        if (position > _effects.size()) {
            return;
        }

        _effects.insert(_effects.begin() + position, EffectInfo(effectId, 1));
    }

    virtual void removeEffect() override {
        internalRemoveEffect();
    }

    virtual void removeEffect(uint32_t position) override {
        if (position >= _effects.size()) {
            return;
        }

        _effects.erase(_effects.begin() + position);

        if (_currentEffectIndex >= _effects.size()) {
            _currentEffectIndex = 0;
        }
    }

    virtual size_t size() const override {
        return _effects.size();
    }

    virtual void reset() override {
        clear();
        createDefaultEffectsList();
    }

    virtual void clear() override {
        _effects.clear();
        _currentEffectIndex = 0;
    }
private:
    void createDefaultEffectsList() {
        // Тут можно комментить не нужные эффекты, чтобы они не попадали в начальный список эффектов
        internalAddEffect(1); // SlowRandom
        internalAddEffect(2); // SimpleRainbow
        internalAddEffect(3); // Dribs
        internalAddEffect(4); // Rain
        internalAddEffect(5); // AllRandom
        internalAddEffect(6); // Snow
        internalAddEffect(7); // Fire
        internalAddEffect(8); // TheMatrix
        internalAddEffect(9); // SimpleBalls
        internalAddEffect(10); // Confetti
        internalAddEffect(11); // Starfall
        internalAddEffect(12); // DynamicSquare
        internalAddEffect(13); // RandomRain
        internalAddEffect(14); // RainbowRain
        internalAddEffect(15); // Points
        internalAddEffect(16); // RainbowPoint
        internalAddEffect(17); // RainbowStaticPoint
        internalAddEffect(18); // Text
        internalAddEffect(19); // Mouse
        internalAddEffect(20); // Pacman
        internalAddEffect(21); // CircularPoint
        internalAddEffect(22); // Zigzag
        internalAddEffect(23); // HorizontalRainbowPoint
        internalAddEffect(24); // Ny2020
        internalAddEffect(25); // DribsAllSide
        internalAddEffect(26); // Snake
        internalAddEffect(27); // RadialFire
        internalAddEffect(28); // RadialPattern
        internalAddEffect(29); // CrazyBees
    }

    void internalAddEffect(uint32_t effectId) {
        // второе значение - это заглушка, т.к. в статическом хранилище не нужно
        // хранить идентификатор эффекта в памяти (1 означает, что эффект валидный)
        _effects.emplace_back(effectId, 1);
    }

    void internalRemoveEffect() {
        if (_effects.empty()) {
            return;
        }

        _effects.pop_back();

        if (_currentEffectIndex >= _effects.size()) {
            _currentEffectIndex = 0;
        }
    }
};
