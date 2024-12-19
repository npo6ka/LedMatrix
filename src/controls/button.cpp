#include "configs/constants.h"
#include "button.h"
#include "events/observer.h"
// GButton(BTN_PIN, LOW_PULL, NORM_OPEN)
Button::Button(int8_t pin, bool type, bool dir) : touch(GButton(pin, type, dir)) {
    touch.setStepTimeout(BUTTON_STEP_TIMEOUT);
    touch.setClickTimeout(BUTTON_CLICK_TIMEOUT);
}

void Button::onTick() {
    touch.tick();
    uint8_t clickCount = touch.hasClicks() ? touch.getClicks() : 0U;
    switch (clickCount) {
        case 1U: {
            //Observable::notify(ChangeModEvent({EventType::ChangeMode, ChangeModEvent::Type::Next, 0}));
            break;
        }
        case 2U: {
            //Observable::notify(Event({EventType::ChangeAutoMod}));
            break;
        }
        default:
            break;
    }
}
