#pragma once

enum class EventType {
  ChangeAutoMod = 0,
  ChangePowerState, // Event
  SetPowerState,    // ChangeBoolEvent
  ChangeBrightness, // ChangeIntEvent
  ChangeSpeed,      // ChangeIntEvent
  ChangeMode,       // ChangeModEvent
  ChangeModVar,     // ChangeModVarEvent
  EventAmount       // Используется, чтобы знать сколько всего ивентов
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

struct ChangeModEvent : public Event {
  enum class Type {
    Next,
    Previous,
    Set // установить конкретный мод по id
  };
  Type type;
  uint8_t id;
  ChangeModEvent(EventType et, Type t, uint8_t i = 0) : Event(et), type(t), id(i) {}
};

struct ChangeModVarEvent : public Event {
  int val;
  int offset;
  ChangeModVarEvent(EventType t, int v, int o) : Event(t), val(v), offset(o) {}
};