#include "myapplication.h"

#include "libs/lib_led.h"
#include "effect_list/effectslist.h"

// все настройки матрицы находятся в lib_led.h
// инициализация светодиодов
CRGB leds[LEDS_CNT];

MyApplication::MyApplication() : _button(Button(BTN_PIN, LOW_PULL, NORM_OPEN)), _power(true) {
    _ir = std::move(IR(IR_RECEIVE_PIN));
    randomSeed(millis() + analogRead(A0));
    random16_set_seed(millis() + analogRead(A0));
    debug_setup();
    led_setup();

    FastLED.setBrightness(150);

    Observable::subscribe(EventType::ChangePowerState, this);
    //Observable::notify(ChangeModEvent({EventType::ChangeMode, ChangeModEvent::Type::Set, 0}));
};

void MyApplication::onTick() {
    if (_ir.isIdle()) {
        if (_power) {
            EffectsList::getInstance().onTick();
        }
        _autoMod.onTick();
        _button.onTick();
        _ir.onTick();
    }
}

void MyApplication::handleEvent(Event *event) {
    if (event->type == EventType::ChangePowerState) {
        ChangeBoolEvent *ev = static_cast<ChangeBoolEvent *>(event);
        _power = ev->new_val;
    }
}