#pragma once

#include "configs/constants.h"
#include "events/observer.h"
#include "controls/automode.h"
#include "core/effect/storage/IEffectStorage.h"
#include "core/effect/EffectManager.h"
#include "core/input/InputHub.h"

#if BTN_ENABLE
#   include "controls/button.h"
#endif
#if IR_ENABLE
#   include "controls/ir.h"
#endif
#if RELAY_ENABLE
#   include "modules/relay.h"
#endif
#if MIC_ENABLE && defined(ESP32DEV)
#   include "modules/MicInputProvider.h"
#endif
#if WIFI_ENABLE && defined(ESP32DEV)
#   include "controls/webcontrol.h"
#endif

#if SAVE_TO_EEPROM
#   include "core/file/IFileHandler.h"
#   include "core/Variable/FileSavableVariable.h"
#endif

class MyApplication : public IObserver {
private:
    bool _isPowerOn;
    AutoChangeMode _autoMod;
    std::unique_ptr<IEffectStorage> _effectStorage;
    std::unique_ptr<EffectManager> _effectManager;
    InputHub _inputHub;
#if SAVE_TO_EEPROM
    std::unique_ptr<IFileHandler> _brightnessFile;
    std::unique_ptr<FileSavableVariable<uint8_t>> _savedBrightness;
#endif
#if BTN_ENABLE
    Button _button;
#endif
#if IR_ENABLE
    IR _ir;
#endif
#if RELAY_ENABLE
    Relay _relay;
#endif
#if MIC_ENABLE && defined(ESP32DEV)
    MicInputProvider _micProvider;
#endif
#if WIFI_ENABLE && defined(ESP32DEV)
    WebControl _webControl;
#endif

    MyApplication(const MyApplication &) = delete;
    MyApplication& operator=(const MyApplication& other) = delete;
    MyApplication(MyApplication&& other) = delete;
    MyApplication& operator=(MyApplication&& other) = delete;

    void setPowerState(bool state);
public:
    MyApplication();
    ~MyApplication();
    void onTick();
    void onInit();
    virtual void handleEvent(const Event *event) override;

    bool isPowerOn() const { return _isPowerOn; }
    bool isAutoMode() const { return _autoMod.isEnable(); }
};
