#include "configs/constants.h"
#include "ir.h"
#include "control.h"
#include <NecDecoder.h>



class IR: public IControl {
    NecDecoder ir;
public:
    IR() {
        attachInterrupt(IR_PIN, (void(*)())(irIsr), FALLING);
    }

    // в прерывании вызываем tick()
    void ICACHE_RAM_ATTR irIsr() {
        ir.tick();
    }

    ControlState tick() final {
        if (ir.available()) {
            switch (ir.readCommand()) {
                case 0x18:
                    Serial.print("↑\n");
                    break;
                case 0x4a:
                    Serial.print("↓\n");
                    break;
                case 0x5a:
                    Serial.print("→\n");
                    break;
                case 0x10:
                    Serial.print("←\n");
                    break;
                case 0x38:
                    Serial.print("OK\n");
                    break;
            }
        }
    }
};





void ir_setup() {
    // подключил на D2, прерывание 0
    
}

/*
    китайский чёрный пульт с бело/красно/синими кнопками
    ↑ 0x18
    ↓ 0x4a
    → 0x5a
    ← 0x10
    OK 0x38

    * 0x68
    # 0xB0
    0 0x98
    1 0xa2
    2 0x62
    3 0xe2
    4 0x22
    5 0x2
    6 0xc2
    7 0xe0
    8 0xa8
    9 0x90
*/
}

