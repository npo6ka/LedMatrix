# pragma once

#include "IEffectStorage.h"

#include "configs/DefaultEffectList.h"
#include "core/effect/EffectInfo.h"
#include "libs/debug_lib.h"

#include "vector"

class StaticEffectStorage : public IEffectStorage
{
private:
    std::vector<EffectInfo> _effects;
    uint32_t _currentEffectIndex = 0;
public:
    StaticEffectStorage() {
        createDefaultEffectsList();
        _currentEffectIndex = _effects.size() - 1;
    }

    virtual ~StaticEffectStorage() {};

    virtual const EffectInfo& getEffectInfo(uint32_t index) const override{
        if (index >= _effects.size()) {
            logError("Cannot get effect info: index is out of range\n");
            return EffectInfo::getErrorEffectInfo();
        }
        return _effects[index];
    }

    virtual uint32_t getCurrentIndex() const override{
        return _currentEffectIndex;
    }

    virtual void setCurrentIndex(uint32_t index) override{
        if (index >= _effects.size()) {
            logError("Cannot set current index: index is out of range\n");
            return;
        }
        _currentEffectIndex = index;
    }

    virtual void addEffect(uint32_t effectId) override {
        internalAddEffect(effectId);
    }

    virtual void addEffect(uint32_t effectId, uint32_t position) override{
        if (position > _effects.size()) {
            logError("Cannot add effect: position is out of range\n");
            return;
        }

        _effects.insert(_effects.begin() + position, EffectInfo(effectId, 1));
    }

    virtual void removeEffect() override {
        internalRemoveEffect();
    }

    virtual void removeEffect(uint32_t position) override {
        if (position >= _effects.size()) {
            logError("Cannot remove effect: position is out of range\n");
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
        for (EffectId id : DefaultEffects::effectIds) {
            internalAddEffect(static_cast<uint32_t>(id));
        }
    }

    void internalAddEffect(uint32_t effectId) {
        // второе значение - это заглушка, т.к. в статическом хранилище не нужно
        // хранить идентификатор эффекта в памяти (1 означает, что эффект валидный)
        _effects.emplace_back(effectId, 1);
    }

    void internalRemoveEffect() {
        if (_effects.empty()) {
            logError("Cannot remove effect: effects list is empty\n");
            return;
        }

        _effects.pop_back();

        if (_currentEffectIndex >= _effects.size()) {
            _currentEffectIndex = 0;
        }
    }
};
