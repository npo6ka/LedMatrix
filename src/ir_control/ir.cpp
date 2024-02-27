#include "configs/constants.h"
#include "ir.h"

#if IR_ENABLE
#include "libs/debug_lib.h"
#undef DEBUG
// #include "TinyIRReceiver.hpp"
#include <IRremote.hpp>

void ir_setup() {
    IrReceiver.begin(IR_RECEIVE_PIN, DISABLE_LED_FEEDBACK); // Start the receiver
}

void ir_tick() {
    if (IrReceiver.decode()) {
        switch (IrReceiver.decodedIRData.command) {
            case 0x45: // <<
                // prev mode
                out("prev mode\n");
                break;
            case 0x47: // play
                out("turn on\n");
                // turn on
                break;
            case 0x4a: // ||
                out("turn off\n");
                // turn off
                break;
            case 0x48: // >>
                out("next mode\n");
                // next mode
                break;
        }
        IrReceiver.resume(); // Enable receiving of the next value
    }
}
#else
void ir_setup() {}
void ir_tick() {}
#endif