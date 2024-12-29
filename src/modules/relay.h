#pragma once
#include "stdint.h"

class Relay {
    uint8_t _pin;
    unsigned long _time;
    bool *_powerState;
    bool _relayState;
public:
    Relay(uint8_t pin, bool *powerState) : _pin(pin), _time(0), _powerState(powerState), _relayState(true) {}

    void onInit();
    void onTick();
};