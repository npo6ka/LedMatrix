#include "configs/constants.h"
#include "ir.h"

#if IR_ENABLE
#include "libs/debug_lib.h"
#undef DEBUG
// #include "TinyIRReceiver.hpp"
#include <IRremote.hpp>

IR::IR() {
    IrReceiver.begin(IR_RECEIVE_PIN, DISABLE_LED_FEEDBACK); // Start the receiver
}

IR *IR::instance() {
    static IR ins;
    return &ins;
}

ControlState IR::tick() {
    ControlState state = ControlState::None;
    if (IrReceiver.decode()) {
        switch (IrReceiver.decodedIRData.command) {
            case 0x45: // <<
                state = ControlState::PrevMode;
                break;
            case 0x47: // play
                state = ControlState::PowerOn;
                break;
            case 0x4a: // ||
                state = ControlState::PowerOff;
                break;
            case 0x48: // >>
                state = ControlState::NextMode;
                break;
        }
        IrReceiver.resume(); // Enable receiving of the next value
    }
    return state;
}
#else
IR::IR() {}
IR *IR::instance() {
    return nullptr;
}
ControlState IR::tick() {
    return ControlState::None;
}
#endif