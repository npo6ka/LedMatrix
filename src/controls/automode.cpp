#include "configs/constants.h"
#include "libs/debug_lib.h"
#include "automode.h"
#include <Arduino.h>

AutoChangeMode::AutoChangeMode(bool isEnable, unsigned long delay) : _isEnable(false), _delay(delay) {
    setIsEnable(isEnable);
    Observable::subscribe(EventType::ChangeMode, this);
    Observable::subscribe(EventType::ChangeAutoMod, this);
    Observable::subscribe(EventType::ChangePowerState, this);
}

AutoChangeMode::~AutoChangeMode() {
    Observable::unsubscribe(EventType::ChangeMode, this);
}

void AutoChangeMode::onTick() {
    if (_isEnable) {
        if (millis() - _savedTime > _delay /*&& EffectsList::getInstance().effectIsEnd()*/) {
            auto ev = ChangeModEvent({EventType::ChangeMode, ChangeModEvent::Type::Next, 0});
            Observable::notify(&ev);
            out("AutoControl: next mode\n");
        }
    }
}

void AutoChangeMode::setIsEnable(bool isEnable) {
    if (isEnable && !_isEnable) { // если переключит с false -> true, сохраняем время
        _savedTime = millis();
    }
    _isEnable = isEnable;
}

void AutoChangeMode::setDelay(unsigned long delay) {
    _delay = delay;
}

void AutoChangeMode::handleEvent(Event *event) {
    if (event->type == EventType::ChangeMode) {
        // сменился мод, сбрасываем таймер
        _savedTime = millis();
    } else if (event->type == EventType::ChangeAutoMod) {
        setIsEnable(!_isEnable);
    } else if (event->type == EventType::ChangePowerState) {
        ChangeBoolEvent *ev = static_cast<ChangeBoolEvent *>(event);
        setIsEnable(ev->new_val);
    }
}