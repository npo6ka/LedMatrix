#include "button/button_handler.h"
#include "ir_control/ir.h"
#include "effect_list/effectslist.h"
#include "effect_list/libs/lib_led.h"

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
    EffectsList::getInstance().onTick();

    tick_buttons();
    ir_tick();
}