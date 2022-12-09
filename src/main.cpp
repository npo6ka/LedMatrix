#include "button/button_handler.h"
#include "effect_list/effectslist.h"
#include "effect_list/libs/lib_led.h"

// все настройки матрицы находятся в lib_led.h
// инициализация светодиодов
CRGB leds[LEDS_CNT];

void setup() {
    randomSeed(millis() + analogRead(A0));
    debug_setup();
    led_setup();
    setup_buttons();

    FastLED.setBrightness(150);
    EffectsList::getInstance().setEffect(0);
}

unsigned long tick = 0;
int tps = 0;

void loop() {
    EffectsList::getInstance().onTick();

    tick_buttons();

    // проверка реального тпс работы микроконтроллера
    /*tps++;
    if (millis() > tick * 1000 ) {
        out("tps: %d fps: %.1f\n", tps, EffectsList::getInstance().getCurFPS());
        tick++;
        tps = 0;
    }*/
}