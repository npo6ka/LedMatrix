#pragma once

#include "core/effect/EffectManager.h"
#include "core/effect/storage/IEffectStorage.h"
#include "events/observer.h"

#include <functional>

struct AppStatusSnapshot {
    bool powerOn = false;
    bool autoMode = false;
    uint16_t effectIndex = 0;
    uint8_t effectId = 0;
    const char* effectName = "";
    float fps = 0.0f;
    uint8_t fpsTarget = 0;
    uint8_t fpsMin = 0;
    uint8_t fpsMax = 0;
    uint8_t brightness = 0;
    bool symmetric = false;
    uint16_t width = 0;
    uint16_t height = 0;
    size_t effectsCount = 0;
    String ip;
    String ssid;
};

class AppStatus : public IObserver {
public:
    using PowerFn = std::function<bool()>;
    using AutoModeFn = std::function<bool()>;
    using BrightnessFn = std::function<uint8_t()>;

    AppStatus();
    ~AppStatus();

    void bind(
        IEffectStorage* storage,
        EffectManager* effectManager,
        PowerFn powerFn,
        AutoModeFn autoModeFn,
        BrightnessFn brightnessFn);

    void setNetworkInfo(const String& ip, const String& ssid);
    void updateFps();
    AppStatusSnapshot snapshot() const;
    IEffectStorage* storage() const { return _storage; }

    void handleEvent(const Event* event) override;

private:
    IEffectStorage* _storage = nullptr;
    EffectManager* _effectManager = nullptr;
    PowerFn _powerFn;
    AutoModeFn _autoModeFn;
    BrightnessFn _brightnessFn;

    uint16_t _effectIndex = 0;
    uint8_t _effectId = 0;
    float _fps = 0.0f;
    String _ip;
    String _ssid;
};
