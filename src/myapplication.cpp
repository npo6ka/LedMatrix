#include "myapplication.h"

#include "core/effect/EffectManager.h"
#include "libs/StdFeatures.h"

#if SAVE_TO_EEPROM
#   include "core/file/LsfFileHandler.h"
#   include "core/effect/storage/FileEffectStorage.h"
#else
#   include "core/effect/storage/StaticEffectStorage.h"
#endif

MyApplication::MyApplication() :
        _isPowerOn(true),
        _autoMod(AUTOMOD_DEF_STATE, _isPowerOn, AUTOMOD_INTERVAL)
#if BTN_ENABLE
        , _button(BTN_PIN, LOW_PULL, NORM_OPEN)
#endif
#if IR_ENABLE
        , _ir()
#endif
#if RELAY_ENABLE
        , _relay(RELAY_PIN, &_isPowerOn)
#endif
{
    Observable::subscribe(EventType::ChangePowerState, this);
    Observable::subscribe(EventType::SetPowerState, this);
    Observable::subscribe(EventType::ChangeMode, this);
};

MyApplication::~MyApplication() {
    Observable::unsubscribe(EventType::ChangePowerState, this);
    Observable::unsubscribe(EventType::SetPowerState, this);
    Observable::unsubscribe(EventType::ChangeMode, this);
}

// лучше всё по максимому инициализировать тут
void MyApplication::onInit() {
    randomSeed(millis() + analogRead(A0));
    random16_set_seed(millis() + analogRead(A0));
    debugSetup();
    LedMatrix.setup();
#if IR_ENABLE
    _ir.onInit(IR_RECEIVE_PIN);
#endif
#if RELAY_ENABLE
    _relay.onInit();
#endif
#if SAVE_TO_EEPROM
    _effectStorage = std::make_unique<FileEffectStorage>(std::make_unique<LsfFileHandler>(SAVE_TO_EEPROM_FILE));
#else
    _effectStorage = std::make_unique<StaticEffectStorage>();
#endif
    _effectManager = std::make_unique<EffectManager>(*_effectStorage.get());
}

void MyApplication::onTick() {
#if IR_ENABLE
    if (_ir.isIdle())
#endif
    {
        if (_isPowerOn) {
            _effectManager->onTick();
            _autoMod.onTick();
        }
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

void MyApplication::setPowerState(bool newState) {
    if (_isPowerOn == newState) return;

    _isPowerOn = newState;
    _autoMod.setPowerState(newState);
    if (!_isPowerOn) {
        // Очистить матрицу
        FastLED.clear();
        FastLED.show();
    }
}

void MyApplication::handleEvent(Event *event) {
    if (event->type == EventType::ChangePowerState) {
        setPowerState(!_isPowerOn);
    } else if (event->type == EventType::SetPowerState) {
        ChangeBoolEvent *ev = static_cast<ChangeBoolEvent *>(event);
        setPowerState(ev->new_val);
    } else if (event->type == EventType::ChangeAutoMod) {
        ChangeBoolEvent *ev = static_cast<ChangeBoolEvent *>(event);
        _autoMod.setIsEnable(ev->new_val);
    } else if (event->type == EventType::ChangeMode) { // включить питание при попытках сменить режима
        if (!_isPowerOn) {
            setPowerState(true);
        }
    }
}