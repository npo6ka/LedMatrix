#include "button_handler.h"
#include "GyverButton.h"
#include "debug_lib.h"
#include "effects/effectslist.h"

#define BTN_PIN               (4U)      // пин кнопки (D2)
#define BUTTON_STEP_TIMEOUT   (100U)    // каждые BUTTON_STEP_TIMEOUT мс будет генерироваться событие удержания кнопки (для регулировки яркости)
#define BUTTON_CLICK_TIMEOUT  (500U)    // максимальное время между нажатиями кнопки в мс, до достижения которого считается серия последовательных нажатий

#define AUTOMOD_INTERVAL      (5000U)    // кол-во времни между автоматическим переключением режима

GButton touch(BTN_PIN, LOW_PULL, NORM_OPEN);

bool auto_mode = false;
long int auto_mode_cnt = 0;

void setup_buttons() {
    touch.setStepTimeout(BUTTON_STEP_TIMEOUT);
    touch.setClickTimeout(BUTTON_CLICK_TIMEOUT);
}

void auto_mode_tick() {
    if (millis() - auto_mode_cnt > AUTOMOD_INTERVAL) {
        auto_mode_cnt = millis();
        EffectsList::getInstance().nextEffect();
        out("auto next mode");
    }
}

void tick_buttons() {
    touch.tick();
    uint8_t clickCount = touch.hasClicks() ? touch.getClicks() : 0U;
    switch (clickCount)
    {
    case 1U:
        if (auto_mode) {
            auto_mode = false;
            out("auto mode off");
        } else {
            EffectsList::getInstance().nextEffect();
            out("next mode");
        }
        break;
    case 2U:
        if (!auto_mode) {
            auto_mode = true;
            auto_mode_cnt = millis();
            out("auto mode on");
        }
        break;
    
    default:
        break;
    }
    if (auto_mode) auto_mode_tick();
}