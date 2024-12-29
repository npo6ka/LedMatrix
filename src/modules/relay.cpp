#include "relay.h"

#include "Arduino.h"
#include "configs/constants.h"

void Relay::onInit() {
    pinMode(_pin, OUTPUT);
}

void Relay::onTick() {
    if (_time == 0) {
        if (*_powerState != _relayState) {
            Serial.printf("change power\n");
            digitalWrite(_pin, HIGH);
            _time = millis();
            _relayState = *_powerState;
        }
    } else if (millis() - _time > RELAY_DELAY) {
        digitalWrite(_pin, LOW);
        _time = 0;
    }
}