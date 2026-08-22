#include "myapplication.h"

#include "core/effect/EffectManager.h"
#include "libs/StdFeatures.h"
#include "libs/led_matrix.h"

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
        , _button(BTN_PIN, HIGH_PULL, NORM_OPEN)
#endif
#if IR_ENABLE
        , _ir()
#endif
#if RELAY_ENABLE
        , _relay(RELAY_PIN, &_isPowerOn)
#endif
#if MIC_ENABLE && defined(ESP32DEV)
        , _micProvider()
#endif
{
    Observable::subscribe(EventType::ChangePowerState, this);
    Observable::subscribe(EventType::SetPowerState, this);
    Observable::subscribe(EventType::ChangeMode, this);
    Observable::subscribe(EventType::ResetModesList, this);
    Observable::subscribe(EventType::SetAutoMod, this);
    Observable::subscribe(EventType::SetBrightness, this);
};

MyApplication::~MyApplication() {
    Observable::unsubscribe(EventType::ChangePowerState, this);
    Observable::unsubscribe(EventType::SetPowerState, this);
    Observable::unsubscribe(EventType::ChangeMode, this);
    Observable::unsubscribe(EventType::ResetModesList, this);
    Observable::unsubscribe(EventType::SetAutoMod, this);
    Observable::unsubscribe(EventType::SetBrightness, this);
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
#if RESET_EFFECTS_LIST_ON_BOOT
    _effectStorage->reset();
#endif
#if MIC_ENABLE && defined(ESP32DEV)
    _inputHub.registerProvider(&_micProvider);
#endif
    _effectManager = std::make_unique<EffectManager>(*_effectStorage.get(), _inputHub);
#if WIFI_ENABLE && defined(ESP32DEV)
    _webControl.onInit(_effectStorage.get(), _effectManager.get(), &_isPowerOn, &_autoMod);
#endif
}

void MyApplication::onTick() {
#if WIFI_ENABLE && defined(ESP32DEV)
    _webControl.onTick();
#endif
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
#if WIFI_ENABLE && defined(ESP32DEV)
    yield();
#endif
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

void MyApplication::handleEvent(const Event *event) {
    if (event->type == EventType::ChangePowerState) {
        setPowerState(!_isPowerOn);
    } else if (event->type == EventType::SetPowerState) {
        const ChangeBoolEvent *ev = static_cast<const ChangeBoolEvent *>(event);
        setPowerState(ev->new_val);
    } else if (event->type == EventType::SetAutoMod) {
        const ChangeBoolEvent *ev = static_cast<const ChangeBoolEvent *>(event);
        _autoMod.setIsEnable(ev->new_val);
    } else if (event->type == EventType::SetBrightness) {
        const ChangeIntEvent *ev = static_cast<const ChangeIntEvent *>(event);
        int value = ev->new_val;
        if (value < 0) value = 0;
        if (value > 255) value = 255;
        LedMatrix.setBrightness(static_cast<uint8_t>(value));
    } else if (event->type == EventType::ChangeMode) { // включить питание при попытках сменить режима
        if (!_isPowerOn) {
            setPowerState(true);
        }
    } else if (event->type == EventType::ResetModesList) {
        _effectStorage->reset();
        _effectManager->setEffect(_effectStorage->size() - 1);
    }
}
