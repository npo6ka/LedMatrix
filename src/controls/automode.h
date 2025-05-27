#pragma once
#include "events/observer.h"

class AutoChangeMode: public IObserver {
    bool _isEnable = false;
    bool _isPowerOn = false;
    unsigned long _savedTime;
    unsigned long _delay;

    void beforeChangeStates(bool isEnable, bool isPowerOn);
public:
    // Создание автоматического переключения режимов
    // default_is_on - включено ли переключение режимов по умолчанию
    // delay - задержка переключения режимов
    // переключение режимов может происходить дольше, если у текущего режима используется флаг is_end
    AutoChangeMode(bool isEnable, bool isPowerOn, unsigned long delay);
    ~AutoChangeMode();

    // Тик для автоматического переключения режимов
    void onTick();

    // Установка состояния автоматического переключения режимов
    // true - автопереключение включено
    void setIsEnable(bool isEnableState);

    // Установка состояния питания матрицы для корректного
    // управления режимами когда питание отключено
    void setPowerState(bool isPowerOn);

    // Установка времени между переключениями режимов
    void setDelay(unsigned long delay);

    // Получение текущего состояния автоматического переключения режимов
    bool isEnable();

    virtual void handleEvent(Event *event) override;
};