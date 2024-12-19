#pragma once

enum class EventType {
  ChangeAutoMod = 0,
  ChangeMode,
  ChangePowerState,
  ChangeBrightness,
  ChangeSpeed,
  ChangeModVar,
  EventAmount // используется, чтобы знать сколько всего ивентов
};

struct Event {
  EventType type;
};

struct ChangeBoolEvent : public Event {
  bool new_val;
};

struct ChangeIntEvent : public Event {
  int new_val;
};

struct ChangeModEvent : public Event {
  enum class Type {
    Next,
    Previous,
    Set // установить конкретный мод по id
  };
  Type type;
  uint8_t id;
};

struct ChangeModVarEvent : public Event {
  int new_val;
  int var_offset;
};