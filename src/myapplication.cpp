#include "myapplication.h"

#include "libs/led_matrix.h"
#include "effect_list/effectslist.h"

MyApplication::MyApplication() :
        _autoMod(AUTOMOD_DEF_STATE, AUTOMOD_INTERVAL),
        _power(true)
#if BTN_ENABLE
        , _button(BTN_PIN, LOW_PULL, NORM_OPEN)
#endif
#if IR_ENABLE
        , _ir()
#endif
#if RELAY_ENABLE
        , _relay(RELAY_PIN, &_power)
#endif
{
    Observable::subscribe(EventType::ChangePowerState, this);
};

// лучше всё по максимому инициализировать тут
void MyApplication::onInit() {
    randomSeed(millis() + analogRead(A0));
    random16_set_seed(millis() + analogRead(A0));
    debug_setup();
    LedMatrix.setup();
#if IR_ENABLE
    _ir.onInit(IR_RECEIVE_PIN);
#endif
#if RELAY_ENABLE
    _relay.onInit();
#endif

    EffectsList::getInstance(); // инициализируем EffectsList, чтобы сработало уведомление о новом режиме
    auto ev = ChangeModEvent({EventType::ChangeMode, ChangeModEvent::Type::Set, 0});
    Observable::notify(&ev);
}

void MyApplication::onTick() {
#if IR_ENABLE
    if (_ir.isIdle())
#endif
    {
        if (_power) {
            EffectsList::getInstance().onTick();
        }
        _autoMod.onTick();
#if BTN_ENABLE
        _button.onTick();
#endif
#if IR_ENABLE
        _ir.onTick();
#endif
#if RELAY_ENABLE
        _relay.onTick();
#endif
    }
}

void MyApplication::handleEvent(Event *event) {
    if (event->type == EventType::ChangePowerState) {
        ChangeBoolEvent *ev = static_cast<ChangeBoolEvent *>(event);
        _power = !_power; //_power = ev->new_val; нужно подумать как пофиксить
        if (!_power) {
            FastLED.clear();
            FastLED.show();
        }
    }
}