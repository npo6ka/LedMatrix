#include "configs/constants.h"
#include "ir.h"
#include "libs/debug_lib.h"
#undef DEBUG
// #include "TinyIRReceiver.hpp"
#include <IRremote.hpp>
#include "events/observer.h"

void IR::onInit(int pin) {
    IrReceiver.begin(pin, DISABLE_LED_FEEDBACK); // Start the receiver
}

void IR::onTick() {
    if (IrReceiver.decode()) {
        switch (IrReceiver.decodedIRData.command) {
            case 0x45: { // <<
                Observable::notify<ChangeModeEvent>(EventType::ChangeMode, true, ChangeModeEventRequest::Type::Previous);
                break;
            }
            case 0x47: { // play
                Observable::notify<ChangeBoolEvent>(EventType::SetPowerState, true);
                break;
            }
            case 0x4a: { // ||
                Observable::notify<ChangeBoolEvent>(EventType::SetPowerState, false);
                break;
            }
            case 0x48: { // >>
                Observable::notify<ChangeModeEvent>(EventType::ChangeMode, true, ChangeModeEventRequest::Type::Next);
                break;
            }
        }
        IrReceiver.resume(); // Enable receiving of the next value
    }
}

bool IR::isIdle() {
    return IrReceiver.isIdle();
}