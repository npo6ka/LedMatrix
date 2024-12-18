#include "observer.h"


int Observable::type_to_int(EventType etype) {
  if (etype >= EventType::ChangeAutoMod && etype < EventType::EventAmount) {
    return static_cast<int>(etype);
  } else {
    out("Error check EventType: out of range\n");
    return 0;
  }
}

Observable::Observable() {
  int eventAmount = static_cast<int>(EventType::EventAmount);
  _observerList = new std::pair<int, IObserver **>[eventAmount];
  for (int i = 0; i < eventAmount; i++) {
    _observerList[i].first = 0;
    _observerList[i].second = nullptr;
  }
}

Observable::~Observable() {
  int eventAmount = static_cast<int>(EventType::EventAmount);
  for (int i = 0; i < eventAmount; ++i) {
    if (_observerList[i].second) {
      free(_observerList[i].second);
    }
  }
  free(_observerList);
}


Observable &Observable::instance() {
  static Observable singleton;
  return singleton;
}

void Observable::instanceAddObserver(EventType etype, IObserver *observer) {
  int type = type_to_int(etype);
  // ищем пустую ячейку для записи
  int empty_location = -1;
  int observer_location = -1;
  for (int i = 0; i < _observerList[type].first; ++i) {
    if (_observerList[type].second[i] == nullptr) {
      empty_location = i;
    }
    if (_observerList[type].second[i] == observer) {
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
    _observerList[type].first++;
    _observerList[type].second =
        (IObserver **)realloc(_observerList[type].second,
                              sizeof(IObserver *) * _observerList[type].first);
    _observerList[type].second[_observerList[type].first - 1] = observer;
  } else {
    // если нашли пустую ячейку, то записываем туда
    _observerList[type].second[empty_location] = observer;
  }
}

void Observable::instanceRemoveObserver(EventType etype, IObserver *observer) {
  int type = type_to_int(etype);
  int observer_location = -1;

  for (int i = 0; i < _observerList[type].first; ++i) {
    if (_observerList[type].second[i] == observer) {
      observer_location = i;
      break;
    }
  }

  if (observer_location != -1) {
    _observerList[type].second[observer_location] = nullptr;
  } else {
    out("Error removeObserver: EventType not found\n");
  }
}

void Observable::instanceNotify(EventType etype, Event *event) {
  int type = type_to_int(etype);

  for (int i = 0; i < _observerList[type].first; ++i) {
    if (_observerList[type].second[i]) {
      _observerList[type].second[i]->handleEvent(etype, event);
    }
  }
}

void Observable::addObserver(EventType etype, IObserver *observer) {
  Observable::instance().instanceAddObserver(etype, observer);
}

void Observable::removeObserver(EventType etype, IObserver *observer) {
  Observable::instance().instanceRemoveObserver(etype, observer);
}

void Observable::notify(EventType etype, Event *event) {
  Observable::instance().instanceNotify(etype, event);
}