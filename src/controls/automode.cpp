#include "effect_list/effectslist.h"
#include "configs/constants.h"
#include "libs/debug_lib.h"
#include "automode.h"

AutoControl::AutoControl(bool is_on, unsigned long delay) : _delay(delay) {
    set_state(is_on);
}

void AutoControl::tick() {
    if (_state) {
        if (millis() - _prev_time > _delay && EffectsList::getInstance().effectIsEnd()) {
            _prev_time = millis();
            EffectsList::getInstance().nextEffect();
            out("AutoControl: next mode\n");
        }
    }
}

void AutoControl::set_state(bool is_on) {
    _prev_time = is_on ? millis(): 0;
    _state = is_on;
}


