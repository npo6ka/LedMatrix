#include "stdint.h"
#include "stdio.h"
#include "stdlib.h"
#include <utility>

template <typename... Args> void out(Args... args) { printf(args...); }

// https://ru.wikipedia.org/wiki/%D0%9D%D0%B0%D0%B1%D0%BB%D1%8E%D0%B4%D0%B0%D1%82%D0%B5%D0%BB%D1%8C_(%D1%88%D0%B0%D0%B1%D0%BB%D0%BE%D0%BD_%D0%BF%D1%80%D0%BE%D0%B5%D0%BA%D1%82%D0%B8%D1%80%D0%BE%D0%B2%D0%B0%D0%BD%D0%B8%D1%8F)

enum class EventType {
  ChangeAutoMod = 0,
  ChangeMode,
  ChangePowerState,
  ChangeBrightness,
  ChangeSpeed,
  ChangeModVar,
  EventAmount // используется, чтобы знать сколько всего ивентов
};

// если хотим получать уведомления об ивентах, наследуемся от этого класса и
// реализуем метод handleEvent
class IObserver {
public:
  virtual void handleEvent(EventType EventType) = 0;
};

class Observable {
  std::pair<int, IObserver **> *observerList;

  // проверка корретного типа и возврат id в виде int
  // 0 при ошибке
  int type_to_int(EventType etype) {
    if (etype >= EventType::ChangeAutoMod && etype < EventType::EventAmount) {
      return static_cast<int>(etype);
    } else {
      out("Error check EventType: out of range\n");
      return 0;
    }
  }

  Observable() {
    int eventAmount = static_cast<int>(EventType::EventAmount);
    observerList = new std::pair<int, IObserver **>[eventAmount];
    for (int i = 0; i < eventAmount; i++) {
      observerList[i].first = 0;
      observerList[i].second = nullptr;
    }
  }

  ~Observable() {
    int eventAmount = static_cast<int>(EventType::EventAmount);
    for (int i = 0; i < eventAmount; ++i) {
      if (observerList[i].second) {
        free(observerList[i].second);
      }
    }
    free(observerList);
  }

  Observable(const Observable &) = delete;
  Observable &operator=(const Observable &) = delete;

public:
  static Observable &instance() {
    static Observable singleton;
    return singleton;
  }

  void addObserver(EventType etype, IObserver *observer) {
    int type = type_to_int(etype);
    // ищем пустую ячейку для записи
    int empty_location = -1;
    int observer_location = -1;
    for (int i = 0; i < observerList[type].first; ++i) {
      if (observerList[type].second[i] == nullptr) {
        empty_location = i;
      }
      if (observerList[type].second[i] == observer) {
        observer_location = i;
        break;
      }
    }
    if (observer_location) {
      out("Error addObserver: observer exists on list");
      return;
    }
    // если не нашли пустую ячейку расширяем память
    if (empty_location == -1) {
      observerList[type].first++;
      observerList[type].second =
          (IObserver **)realloc(observerList[type].second,
                                sizeof(IObserver *) * observerList[type].first);
      observerList[type].second[observerList[type].first - 1] = observer;
    } else {
      // если нашли пустую ячейку, то записываем туда
      observerList[type].second[empty_location] = observer;
    }
  }

  void removeObserver(EventType etype, IObserver *observer) {
    int type = type_to_int(etype);
    int observer_location = -1;

    for (int i = 0; i < observerList[type].first; ++i) {
      if (observerList[type].second[i] == observer) {
        observer_location = i;
        break;
      }
    }

    if (observer_location != -1) {
      observerList[type].second[observer_location] = nullptr;
    } else {
      out("Error removeObserver: EventType not found\n");
    }
  }

  void notifyObserver(EventType etype) {
    int type = type_to_int(etype);

    for (int i = 0; i < observerList[type].first; ++i) {
      if (observerList[type].second[i]) {
        observerList[type].second[i]->handleEvent(etype);
      }
    }
  }
};