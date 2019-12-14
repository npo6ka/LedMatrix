#include "button/GyverButton.h"
#include "effects/effectslist.h"

// все настройки матрицы находятся в lib_led.h
// инициализация светодиодов
CRGB leds[LEDS_CNT];

#define BTN_PIN               (4U)      // пин кнопки (D2)
#define BUTTON_STEP_TIMEOUT   (100U)    // каждые BUTTON_STEP_TIMEOUT мс будет генерироваться событие удержания кнопки (для регулировки яркости)
#define BUTTON_CLICK_TIMEOUT  (500U)    // максимальное время между нажатиями кнопки в мс, до достижения которого считается серия последовательных нажатий

#define AUTOMOD_INTERVAL      (5000U)    // кол-во времни между автоматическим переключением режима

EffectsList *effects;
GButton touch(BTN_PIN, LOW_PULL, NORM_OPEN);

void setup() {
    randomSeed(millis());
    debug_setup();
    led_setup();
    
    effects = new EffectsList();
    effects->setEffect(1);

    // КНОПКА
    touch.setStepTimeout(BUTTON_STEP_TIMEOUT);
    touch.setClickTimeout(BUTTON_CLICK_TIMEOUT);
}

unsigned long tick = 0;
int tps = 0;
bool auto_mode = true;
long int auto_mode_cnt = 0;


void buttonTick() {
    touch.tick();
    uint8_t clickCount = touch.hasClicks() ? touch.getClicks() : 0U;
    if (clickCount == 1U) {
        if (auto_mode) {
            auto_mode = false;
            out("auto mode off");
        } else {
            effects->nextEffect();
            out("next mode");
        }
    }
    if (clickCount == 2U) {
        if (!auto_mode) {
            auto_mode = true;
            auto_mode_cnt = millis();
            out("auto mode on");
        }
    }
}

void auto_mode_tick() {
    if (auto_mode && millis() - auto_mode_cnt > AUTOMOD_INTERVAL) {
        auto_mode_cnt = millis();
        effects->nextEffect();
        out("auto next mode");
    }
}

void loop() {
    effects->onTick();

    buttonTick();
    auto_mode_tick();
    
    FastLED.setBrightness(128);

    // проверка реального тпс работы микроконтроллера
    /*tps++;
    if (millis() > tick * 1000 ) {
        out("tps: %d fps: %.1f\n", tps, effects->getCurFPS());
        tick++;
        tps = 0;
    }*/
}