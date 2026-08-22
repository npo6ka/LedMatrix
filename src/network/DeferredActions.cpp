#include "DeferredActions.h"

#include "events/ChangeModeEventRequest.h"
#include "events/observer.h"

QueueHandle_t DeferredActions::_queue = nullptr;

bool DeferredActions::init() {
    if (_queue) {
        return true;
    }
    _queue = xQueueCreate(8, sizeof(DeferredAction));
    return _queue != nullptr;
}

bool DeferredActions::post(const DeferredAction& action) {
    if (!_queue) {
        return false;
    }
    return xQueueSend(_queue, &action, 0) == pdTRUE;
}

void DeferredActions::process() {
    if (!_queue) {
        return;
    }

    DeferredAction action;
    while (xQueueReceive(_queue, &action, 0) == pdTRUE) {
        switch (action.type) {
            case DeferredActionType::ModeNext:
                Observable::notify<ChangeModeEvent>(
                    EventType::ChangeMode, true, ChangeModeEventRequest::Type::Next);
                break;
            case DeferredActionType::ModePrev:
                Observable::notify<ChangeModeEvent>(
                    EventType::ChangeMode, true, ChangeModeEventRequest::Type::Previous);
                break;
            case DeferredActionType::ModeSet:
                Observable::notify<ChangeModeEvent>(
                    EventType::ChangeMode, true, ChangeModeEventRequest::Type::Set, action.data.indexValue);
                break;
            case DeferredActionType::PowerSet:
                Observable::notify<ChangeBoolEvent>(EventType::SetPowerState, action.data.boolValue);
                break;
            case DeferredActionType::AutoModSet:
                Observable::notify<ChangeBoolEvent>(EventType::SetAutoMod, action.data.boolValue);
                break;
            case DeferredActionType::BrightnessSet:
                Observable::notify<ChangeIntEvent>(EventType::SetBrightness, action.data.intValue);
                break;
            case DeferredActionType::SymmetricSet:
                Observable::notify<ChangeBoolEvent>(EventType::SetSymmetric, action.data.boolValue);
                break;
            case DeferredActionType::ResetModesList:
                Observable::notify<Event>(EventType::ResetModesList);
                break;
        }
    }
}
