#pragma once
#include "events/observer.h"
#include "controls/automode.h"
#include "controls/button.h"
#include "controls/ir.h"

class MyApplication : public IObserver {
private:
    AutoChangeMode _autoMod;
    Button _button;
    IR _ir;
    bool _power;

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