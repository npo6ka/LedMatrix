#pragma once

#include <stdint.h>
#include "events/observer.h"
#include "fps_manager.h"

class Effect;
class FPSManager;

class EffectsManager : public IObserver
{
    struct Request {
        enum class Type {
            None,
            Set,
            Next,
            Previous
        };
        Type type;
        uint8_t num;
        bool hardReset;
    };
    void clearEffect();
    void syncEffect();

public:
    EffectsManager();
    ~EffectsManager();
    void init();
    void setErrorEffect();

    uint8_t getEffectId() const;
    uint8_t getEffectNum() const;
    std::string getEffectName() const;

    void setEffect(const uint8_t num);
    void nextEffect();
    void prevEffect();

    void onTick();
    float getCurFPS();
    void handleEvent(Event *event) override;
private:
    Request _reqEffect = { Request::Type::None, 0, false };
    Effect* _effect = nullptr;
    FPSManager _fpsManager;
};
