#pragma once

#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include <utility>

#include "events.h"

// если хотим получать уведомления об ивентах, наследуемся от этого класса и
// реализуем метод handleEvent
class IObserver {
public:
  virtual void handleEvent(Event *event) = 0;
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
  void instanceNotify(Event &event);

public:
  static void subscribe(EventType etype, IObserver *observer);
  static void unsubscribe(EventType etype, IObserver *observer);
  static void notify(Event &event);
};