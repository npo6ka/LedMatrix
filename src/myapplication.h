#pragma once

#include <Arduino.h>
#include "configs/constants.h"
#include "events/observer.h"
#include "controls/automode.h"

#if BTN_ENABLE
#   include "controls/button.h"
#endif
#if IR_ENABLE
#   include "controls/ir.h"
#endif
#if RELAY_ENABLE
#   include "modules/relay.h"
#endif

class MyApplication : public IObserver {
private:
    bool _isPowerOn;
    AutoChangeMode _autoMod;
#if BTN_ENABLE
    Button _button;
#endif
#if IR_ENABLE
    IR _ir;
#endif
#if RELAY_ENABLE
    Relay _relay;
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
    virtual void handleEvent(Event *event) override;
};