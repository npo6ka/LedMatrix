#include "configs/constants.h"
#include "libs/debug_lib.h"
#include "automode.h"
#include <Arduino.h>

AutoChangeMode::AutoChangeMode(bool isEnableState, bool isPowerOn, unsigned long delay) : _delay(delay) {
    setIsEnable(isEnableState);
    setPowerState(isPowerOn);
    Observable::subscribe(EventType::ChangeMode, this);
    Observable::subscribe(EventType::ChangeAutoMod, this);
}

AutoChangeMode::~AutoChangeMode() {
    Observable::unsubscribe(EventType::ChangeMode, this);
    Observable::unsubscribe(EventType::ChangeAutoMod, this);
}

void AutoChangeMode::onTick() {
    if (isEnable()) {
        if (millis() - _savedTime > _delay) {
            auto ev = ChangeModEvent({EventType::ChangeMode, ChangeModEvent::Type::Next, 0, false});
            Observable::notify(&ev);
            out("AutoControl: next mode\n");
        }
    }
}

bool AutoChangeMode::isEnable() {
    return _isEnable && _isPowerOn;
}

void AutoChangeMode::beforeChangeStates(bool isEnableState, bool isPowerOn) {
    // если оба новых состояния true и какое то состояние переключили с false -> true, сохраняем время
    if (isEnableState && isPowerOn && !isEnable()) {
        _savedTime = millis();
    }
}

void AutoChangeMode::setIsEnable(bool isEnableState) {
    beforeChangeStates(isEnableState, _isPowerOn);
    _isEnable = isEnableState;
}

void AutoChangeMode::setPowerState(bool isPowerOn) {
    beforeChangeStates(_isEnable, isPowerOn);
    _isPowerOn = isPowerOn;
}

void AutoChangeMode::setDelay(unsigned long delay) {
    _delay = delay;
}

void AutoChangeMode::handleEvent(Event *event) {
    if (event->type == EventType::ChangeMode) {
        // сменился мод, сбрасываем таймер
        if (isEnable()) {
            _savedTime = millis();
        }
    } else if (event->type == EventType::ChangeAutoMod) {
        setIsEnable(!_isEnable);
    }
}