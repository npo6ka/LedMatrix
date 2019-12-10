#include "button/GyverButton.h"
#include "effects/effectslist.h"

// все настройки матрицы находятся в lib_led.h
// инициализация светодиодов
CRGB leds[LEDS_CNT];

EffectsList *effects;

void setup() {
    randomSeed(millis());
    debug_setup();
    led_setup();
    
    effects = new EffectsList();
    effects->setEffect(0);
}

// проверка реального тпс работы микроконтроллера
/*unsigned long tick = 0;
int tps = 0;/*/

void loop() {
    effects->onTick();
    
    // проверка реального тпс работы микроконтроллера
    /*tps++;
    if (millis() > tick * 1000 ) {
        out("tps: %d fps: %.1f\n", tps, effects->getCurFPS());
        tick++;
        tps = 0;
    }*/
}