#pragma once
#include "events/observer.h"
#include "controls/automode.h"
#include "controls/button.h"
#include "controls/ir.h"
#include "modules/relay.h"
#include "wifi/wifi2024.h"

class MyApplication : public IObserver {
private:
    AutoChangeMode _autoMod;
    Button _button;
    IR _ir;
    bool _power;
    Wifi2024 _wifi2024;
    Relay _relay;

    MyApplication(const MyApplication &) = delete;
    MyApplication& operator=(const MyApplication& other) = delete;
    MyApplication(MyApplication&& other) = delete;
    MyApplication& operator=(MyApplication&& other) = delete;

public:
    MyApplication();
    void onTick();
    void onInit();
    virtual void handleEvent(Event *event) override;
};