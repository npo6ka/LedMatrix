#include "configs/constants.h"
#include "button.h"

Button::Button(int8_t pin, bool type, bool dir) : touch(GButton(BTN_PIN, LOW_PULL, NORM_OPEN)) {
    touch.setStepTimeout(BUTTON_STEP_TIMEOUT);
    touch.setClickTimeout(BUTTON_CLICK_TIMEOUT);
}

ControlState Button::tick() {
    touch.tick();
    uint8_t clickCount = touch.hasClicks() ? touch.getClicks() : 0U;
    switch (clickCount) {
        case 1U:
            return ControlState::NextMode;
        case 2U:
            return ControlState::AutoControlSwitch;
        default:
            return ControlState::None;
    }
}
