#include "EffectManager.h"
#include "libs/debug_lib.h"
#include "libs/led_matrix.h"

EffectManager::EffectManager(IEffectStorage& storage, InputHub& inputHub)
    : _storage(storage),
      _inputHub(inputHub)
{
    Observable::subscribe(EventType::ChangeMode, this);

    this->updateEffect();
}

void EffectManager::onTick() {
    this->onCheckRequestedEffectChange();
    _inputHub.tick();
    this->onTickEffect(_inputHub.snapshot());
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

void EffectManager::onTickEffect(const InputSnapshot& snapshot) {
    if (_currentEffect && _fpsManager.needUpdate()) {
        _currentEffect->on_update(snapshot);
        _currentEffect->on_render();
        LedMatrix.mirror();
        FastLED.show();
    }
}

void EffectManager::updateEffect() {
    const EffectInfo& effectInfo = _storage.getEffectInfo(_storage.getCurrentIndex());
    if (_currentEffect) {
        _currentEffect->on_clear();
    }
    _currentEffect = EffectFactory::createEffect(effectInfo.id);
    _inputHub.setActiveCapabilities(EffectFactory::getRequiredCapabilities(effectInfo.id));
    _currentEffect->on_init();
    applyTargetFps();

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

uint8_t EffectManager::fpsMax() {
    const uint16_t hardwareFps = CLedMatrix::hardwareMaxFps();
    // fps режима хранится в uint8_t, выше 255 поднимать некуда
    return hardwareFps > 255 ? 255 : static_cast<uint8_t>(hardwareFps);
}

uint8_t EffectManager::getEffectFps() const {
    return _currentEffect ? _currentEffect->get_fps() : 0;
}

void EffectManager::setEffectFps(uint8_t fps) {
    if (!_currentEffect) {
        return;
    }

    if (fps < kFpsMin) {
        fps = kFpsMin;
    }
    const uint8_t maxFps = fpsMax();
    if (fps > maxFps) {
        fps = maxFps;
    }
    _currentEffect->set_fps(fps);
    applyTargetFps();
}

void EffectManager::applyTargetFps() {
    if (!_currentEffect) {
        return;
    }

    // Режим мог запросить больше, чем лента успевает отдать
    uint8_t fps = _currentEffect->get_fps();
    const uint8_t maxFps = fpsMax();
    if (fps > maxFps) {
        fps = maxFps;
    }
    if (fps < kFpsMin) {
        fps = kFpsMin;
    }
    _fpsManager.setTargetFPS(fps);
}

void EffectManager::handleEvent(const Event* event) {
    if (event->type == EventType::ChangeMode) {
        const ChangeModeEvent* changeEvent = static_cast<const ChangeModeEvent*>(event);
        _pendingRequest = _pendingRequest + changeEvent->request;
    }
}
