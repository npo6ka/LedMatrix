#include "configs/constants.h"
#include "ir.h"

#if IR_ENABLE
#include <NecDecoder.h>
#include "effect_list/effectslist.h"

NecDecoder ir;

// в прерывании вызываем tick()
static void ICACHE_RAM_ATTR irIsr() {
    ir.tick();
}

void ir_setup() {
    // подключил на D2, прерывание 0
    attachInterrupt(IR_PIN, irIsr, FALLING);
}

void ir_tick() {
    if (ir.available()) {
        switch (ir.readCommand())
        {
        case 0x18:
            Serial.print("↑\n");
            break;
        case 0x4a:
            Serial.print("↓\n");
            break;
        case 0x5a:
            Serial.print("→\n");
            EffectsList::getInstance().nextEffect();
            break;
        case 0x10:
            Serial.print("←\n");
            EffectsList::getInstance().prevEffect();
            break;
        case 0x38:
            Serial.print("OK\n");
            break;
        }
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
#else
void ir_setup() {}
void ir_tick() {}
#endif

