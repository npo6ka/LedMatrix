#include "button/button_handler.h"
#include "effects/effectslist.h"
#include "wifi/WiFi_This.h"

// все настройки матрицы находятся в lib_led.h
// инициализация светодиодов
CRGB leds[LEDS_CNT];

void setup() {
    randomSeed(millis() + analogRead(A0));
    debug_setup();
    led_setup();

    // инициализация кнопок
    setup_buttons();

    FastLED.setBrightness(200);
    EffectsList::getInstance().setEffect(0);

    setupWiFi();
}

unsigned long tick = 0;
int tps = 0;

void loop() {
    EffectsList::getInstance().onTick();

    tick_buttons();

    onTickWiFi();

    // проверка реального тпс работы микроконтроллера
    /*tps++;
    if (millis() > tick * 1000 ) {
        out("tps: %d fps: %.1f\n", tps, EffectsList::getInstance().getCurFPS());
        tick++;
        tps = 0;
    }*/
}