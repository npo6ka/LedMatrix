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
            case 0x45: { // <<
                auto ev = ChangeModEvent({EventType::ChangeMode, ChangeModEvent::Type::Previous});
                Observable::notify(&ev);
                break;
            }
            case 0x47: { // play
                auto ev = ChangeBoolEvent({EventType::ChangePowerState, true});
                Observable::notify(&ev);
                break;
            }
            case 0x4a: { // ||
                auto ev = ChangeBoolEvent({EventType::ChangePowerState, false});
                Observable::notify(&ev);
                break;
            }
            case 0x48: { // >>
                auto ev = ChangeModEvent({EventType::ChangeMode, ChangeModEvent::Type::Next});
                Observable::notify(&ev);
                break;
            }
        }
        IrReceiver.resume(); // Enable receiving of the next value
    }
}

bool IR::isIdle() {
    return IrReceiver.isIdle();
}