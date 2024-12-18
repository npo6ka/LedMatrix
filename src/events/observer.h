#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include <utility>

template <typename... Args> void out(Args... args) { printf(args...); }

enum class EventType {
  ChangeAutoMod = 0,
  ChangeMode,
  ChangePowerState,
  ChangeBrightness,
  ChangeSpeed,
  ChangeModVar,
  EventAmount // используется, чтобы знать сколько всего ивентов
};

struct Event {};
struct ChangeBoolEvent : public Event {
  bool new_val;
  bool old_val;
};
struct ChangeIntEvent : public Event {
  int new_val;
  bool old_val;
};
struct ChangeModVarEvent : public Event {
  int new_val;
  int var_offset;
};

// если хотим получать уведомления об ивентах, наследуемся от этого класса и
// реализуем метод handleEvent
class IObserver {
public:
  virtual void handleEvent(EventType EventType, Event *event) = 0;
};

class Observable {
private:
  std::pair<int, IObserver **> *_observerList;

  // проверка корретного типа и возврат id в виде int. 0 при ошибке
  int type_to_int(EventType etype);

  Observable();
  ~Observable();
  Observable(const Observable &) = delete;
  Observable &operator=(const Observable &) = delete;

  static Observable &instance();

  void instanceAddObserver(EventType etype, IObserver *observer);
  void instanceRemoveObserver(EventType etype, IObserver *observer);
  void instanceNotify(EventType etype, Event *event);

public:
  static void addObserver(EventType etype, IObserver *observer);
  static void removeObserver(EventType etype, IObserver *observer);
  static void notify(EventType etype, Event *event);
};