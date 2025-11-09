#include "EffectManager.h"

#include "libs/debug_lib.h"

#include "Arduino.h"
#include <string>

EffectManager::EffectManager(IEffectStorage& storage)
    : _storage(storage)
{
    Observable::subscribe(EventType::ChangeMode, this);

    this->updateEffect();
}

void EffectManager::onTick() {
    this->onCheckRequestedEffectChange();
    this->onTickEffect();
}

void EffectManager::onCheckRequestedEffectChange() {
    // Обработка отложенного запроса на смену эффекта
    if (_pendingRequest.type != ChangeModeEventRequest::Type::None && (!_currentEffect || _currentEffect->is_end() || _pendingRequest.hardReset)) {
        uint16_t requestedModIndex = 0;
        if (_pendingRequest.type == ChangeModeEventRequest::Type::Set) {
            requestedModIndex = _pendingRequest.modNum;
        } else if (_pendingRequest.type == ChangeModeEventRequest::Type::Next) {
            requestedModIndex = (_pendingRequest.modNumOffset + _storage.getCurrentIndex()) % _storage.size();
        } else if (_pendingRequest.type == ChangeModeEventRequest::Type::Previous) {
            requestedModIndex = (_storage.size() + _storage.getCurrentIndex() - (_pendingRequest.modNumOffset % _storage.size())) % _storage.size();
        }

        if (requestedModIndex != _storage.getCurrentIndex()) {
            _storage.setCurrentIndex(requestedModIndex);
            this->updateEffect();
        }

        _pendingRequest = ChangeModeEventRequest();
    }
}

void EffectManager::onTickEffect() {
    if (_currentEffect && _fpsManager.needUpdate()) {
        _currentEffect->on_update();
        _currentEffect->on_render();
        FastLED.show();
    }
}

void EffectManager::updateEffect() {
    const EffectInfo& effectInfo = _storage.getEffectInfo(_storage.getCurrentIndex());
    if (_currentEffect) {
        _currentEffect->on_clear();
    }
    _currentEffect = EffectFactory::createEffect(effectInfo.id, String(effectInfo.savedIndex).c_str());
    _currentEffect->on_init();
    _fpsManager.setTargetFPS(_currentEffect->get_fps());

    Observable::notify<ModChangedEvent>(EventType::ModChanged, effectInfo.id, _storage.getCurrentIndex());
}

void EffectManager::setEffect(uint32_t index) {
    if (index != _storage.getCurrentIndex()) {
        _storage.setCurrentIndex(index);
        this->updateEffect();
    }
}

float EffectManager::getCurrentFPS() const {
    return _fpsManager.getRealFPS();
}

void EffectManager::handleEvent(const Event* event) {
    if (event->type == EventType::ChangeMode) {
        const ChangeModeEvent* changeEvent = static_cast<const ChangeModeEvent*>(event);
        _pendingRequest = _pendingRequest + changeEvent->request;
        this->onTick();
    }
}
