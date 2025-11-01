#pragma once

#include "ChangeModeEventRequest.h"

enum class EventType {
    ChangeAutoMod = 0,
    ChangePowerState,       // ChangePowerStateEvent
    SetPowerState,          // SetPowerStateEvent
    ChangeBrightness,       // ChangeBrightnessEvent
    ChangeSpeed,            // ChangeSpeedEvent
    ChangeMode,             // ChangeModeEvent
    ModChanged,             // ModChangedEvent
    ChangeModVar,           // ChangeModVarEvent
    EventAmount             // Используется, чтобы знать сколько всего ивентов
};

struct Event {
    EventType type;
    Event(EventType t) : type(t) {}
    virtual ~Event() = default;
};

struct ChangeBoolEvent : public Event {
    bool new_val;
    ChangeBoolEvent(EventType t, bool v) : Event(t), new_val(v) {}
};

struct ChangeIntEvent : public Event {
    int new_val;
    ChangeIntEvent(EventType t, int v) : Event(t), new_val(v) {}
};

struct ChangeModeEvent : public Event {
    ChangeModeEventRequest request;
    ChangeModeEvent(
        EventType et,
        bool hardReset,
        ChangeModeEventRequest::Type type,
        uint16_t modIndex = 0,
        int32_t modOffset = 1) :
            Event(et), request(type, hardReset, modIndex, modOffset) {}
};

struct ModChangedEvent : public Event {
    uint8_t id;
    uint8_t num;
    ModChangedEvent(EventType et, uint8_t i = 0, uint8_t n = 0) : Event(et), id(i), num(n) {}
};
struct ChangeModVarEvent : public Event {
    int val;
    int offset;
    ChangeModVarEvent(EventType t, int v, int o) : Event(t), val(v), offset(o) {}
};