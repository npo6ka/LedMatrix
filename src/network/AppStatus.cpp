#include "AppStatus.h"

#include "core/effect/EffectFactory/EffectFactory.h"
#include "libs/led_matrix.h"

AppStatus::AppStatus() {
    Observable::subscribe(EventType::ModChanged, this);
}

AppStatus::~AppStatus() {
    Observable::unsubscribe(EventType::ModChanged, this);
}

void AppStatus::bind(
    IEffectStorage* storage,
    EffectManager* effectManager,
    PowerFn powerFn,
    AutoModeFn autoModeFn,
    BrightnessFn brightnessFn) {
    _storage = storage;
    _effectManager = effectManager;
    _powerFn = std::move(powerFn);
    _autoModeFn = std::move(autoModeFn);
    _brightnessFn = std::move(brightnessFn);

    if (_storage) {
        _effectIndex = static_cast<uint16_t>(_storage->getCurrentIndex());
        _effectId = _storage->getEffectInfo(_storage->getCurrentIndex()).id;
    }
}

void AppStatus::setNetworkInfo(const String& ip, const String& ssid) {
    _ip = ip;
    _ssid = ssid;
}

void AppStatus::updateFps() {
    if (_effectManager) {
        _fps = _effectManager->getCurrentFPS();
    }
}

AppStatusSnapshot AppStatus::snapshot() const {
    AppStatusSnapshot status;
    status.powerOn = _powerFn ? _powerFn() : false;
    status.autoMode = _autoModeFn ? _autoModeFn() : false;
    status.effectIndex = _effectIndex;
    status.effectId = _effectId;
    status.effectName = EffectFactory::getEffectName(_effectId);
    status.fps = _fps;
    status.brightness = _brightnessFn ? _brightnessFn() : 0;
    status.symmetric = LedMatrix.isSymmetric();
    status.width = LedMatrix.width();
    status.height = LedMatrix.height();
    status.effectsCount = _storage ? _storage->size() : 0;
    status.ip = _ip;
    status.ssid = _ssid;
    return status;
}

void AppStatus::handleEvent(const Event* event) {
    if (event->type != EventType::ModChanged) {
        return;
    }

    const ModChangedEvent* modEvent = static_cast<const ModChangedEvent*>(event);
    _effectId = modEvent->id;
    _effectIndex = modEvent->num;
}
