#include "configs/constants.h"
#include "ir.h"

//#if IR_ENABLE
#include "libs/debug_lib.h"
#undef DEBUG
// #include "TinyIRReceiver.hpp"
#include <IRremote.hpp>

class IR: public IControl {
public:
    IR() {
        IrReceiver.begin(IR_RECEIVE_PIN, DISABLE_LED_FEEDBACK); // Start the receiver
    }

    ControlState tick() final {
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
        }
    }
};