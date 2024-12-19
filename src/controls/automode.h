#pragma once
#include "configs/constants.h"
#include "events/observer.h"

class AutoChangeMode: public IObserver {
    bool _isEnable;
    unsigned long _savedTime;
    unsigned long _delay;
public:
    // Создание автоматического переключения режимов
    // default_is_on - включено ли переключение режимов по умолчанию
    // delay - задержка переключения режимов
    // переключение режимов может происходить дольше, если у текущего режима используется флаг is_end
    AutoChangeMode(bool isEnable = AUTOMOD_DEF_STATE, unsigned long delay = AUTOMOD_INTERVAL);
    ~AutoChangeMode();

    // Тик для автоматического переключения режимов
    void onTick();

    // Установка состояния автоматического переключения режимов
    // true - автопереключение включено
    void setIsEnable(bool isEnable);

    // Установка времени между переключениями режимов
    void setDelay(unsigned long delay);

    virtual void handleEvent(Event *event) override;
};