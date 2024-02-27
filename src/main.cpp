#include "ir_control/ir.h"
#include "button_control/button.h"
#include "effect_list/effectslist.h"
#include "libs/lib_led.h"
#if IR_ENABLE
#   include <IRremoteInt.h>
#endif

// все настройки матрицы находятся в lib_led.h
// инициализация светодиодов
CRGB leds[LEDS_CNT];

void setup() {
    randomSeed(millis() + analogRead(A0));
    random16_set_seed(millis() + analogRead(A0));
    debug_setup();
    led_setup();
    setup_buttons();

    FastLED.setBrightness(150);
    EffectsList::getInstance().setEffect(0);
    ir_setup();
}

void loop() {
#if IR_ENABLE
    if (IrReceiver.isIdle())
#endif
    EffectsList::getInstance().onTick();

    tick_buttons();
    ir_tick();
}
