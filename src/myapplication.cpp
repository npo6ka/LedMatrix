#include "myapplication.h"

#include "libs/lib_led.h"
#include "effect_list/effectslist.h"

// все настройки матрицы находятся в lib_led.h
// инициализация светодиодов
CRGB leds[LEDS_CNT];

MyApplication::MyApplication() :
        _autoMod(AUTOMOD_DEF_STATE, AUTOMOD_INTERVAL),
        _button(BTN_PIN, LOW_PULL, NORM_OPEN),
        _ir(),
        _power(true) {
    Observable::subscribe(EventType::ChangePowerState, this);
};

// лучше всё по максимому инициализировать тут
void MyApplication::onInit() {
    randomSeed(millis() + analogRead(A0));
    random16_set_seed(millis() + analogRead(A0));
    debug_setup();
    led_setup();
    _ir.onInit(IR_RECEIVE_PIN);

    EffectsList::getInstance(); // инициализируем EffectsList, чтобы сработало уведомление о новом режиме
    auto ev = ChangeModEvent({EventType::ChangeMode, ChangeModEvent::Type::Set, 0});
    Observable::notify(&ev);
}

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
        if (!_power) {
            FastLED.clear();
            FastLED.show();
        }
    }
}