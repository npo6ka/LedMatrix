# pragma once

#include "core/effect/EffectInfo.h"

#include <stdint.h>

class IEffectStorage
{
public:
    virtual ~IEffectStorage() = default;

    virtual const EffectInfo& getEffectInfo(uint32_t index) const = 0;
    virtual uint32_t getCurrentIndex() const = 0;
    virtual void setCurrentIndex(uint32_t index) = 0;
    virtual void addEffect(uint32_t effectId) = 0;
    // не очень эффективная функция, лучше не использовать
    virtual void addEffect(uint32_t effectId, uint32_t position) = 0;
    virtual void removeEffect() = 0;
    // не очень эффективная функция, лучше не использовать
    virtual void removeEffect(uint32_t position) = 0;
    virtual size_t size() const = 0;
    virtual void reset() = 0;
    virtual void clear() = 0;
};
