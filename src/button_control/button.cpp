#include "configs/constants.h"
#include "button.h"

#if BTN_ENABLE
#include "GyverButton.h"
#include "libs/debug_lib.h"
#include "effect_list/effectslist.h"

GButton touch(BTN_PIN, LOW_PULL, NORM_OPEN);

bool auto_mode = DEFAULT_AUTOMOD;
long int auto_mode_cnt = 0;

void setup_buttons() {
    touch.setStepTimeout(BUTTON_STEP_TIMEOUT);
    touch.setClickTimeout(BUTTON_CLICK_TIMEOUT);
}

static void auto_mode_tick() {
    if (millis() - auto_mode_cnt > AUTOMOD_INTERVAL) {
        auto_mode_cnt = millis();
        EffectsList::getInstance().nextEffect();
        out("auto next mode\n");
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
            out("auto mode off\n");
        } else {
            EffectsList::getInstance().nextEffect();
            out("next mode\n");
        }
        break;
    case 2U:
        if (!auto_mode) {
            auto_mode = true;
            auto_mode_cnt = millis();
            out("auto mode on\n");
        }
        break;
    default:
        break;
    }
    if (auto_mode) auto_mode_tick();
}
#else
void setup_buttons() {}
void tick_buttons() {}
#endif