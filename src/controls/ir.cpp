#include "configs/constants.h"
#include "ir.h"
#include "libs/debug_lib.h"
#undef DEBUG
// #include "TinyIRReceiver.hpp"
#include <IRremote.hpp>
#include "events/observer.h"

IR::IR(uint8_t pin) {
    IrReceiver.begin(pin, DISABLE_LED_FEEDBACK); // Start the receiver
}

void IR::onTick() {
    if (IrReceiver.decode()) {
        switch (IrReceiver.decodedIRData.command) {
            case 0x45: // <<
                // Observable::notify(ChangeModEvent({EventType::ChangeMode, ChangeModEvent::Type::Previous, 0}));
                break;
            case 0x47: // play
                // Observable::notify(ChangeBoolEvent({EventType::ChangePowerState, true}));
                break;
            case 0x4a: // ||
                // Observable::notify(ChangeBoolEvent({EventType::ChangePowerState, false}));
                break;
            case 0x48: // >>
                // Observable::notify(ChangeModEvent({EventType::ChangeMode, ChangeModEvent::Type::Next, 0}));
                break;
        }
        IrReceiver.resume(); // Enable receiving of the next value
    }
}

bool IR::isIdle() {
    return IrReceiver.isIdle();
}