#pragma once

class AutoControl {
    bool _state; // = AUTOMOD_DEF_STATE;
    unsigned long _prev_time; // = 0;
    unsigned long _delay;// = AUTOMOD_INTERVAL;
public:
    // Создание автоматического переключения режимов
    // default_is_on - включено ли переключение режимов по умолчанию
    // delay - задержка переключения режимов
    // переключение режимов можетпроисходить дольше, если у текущего режима используется флаг is_end
    AutoControl(bool is_on, unsigned long delay);
    ~AutoControl() {}

    // Тик для автоматического переключения режимов
    void tick();

    // Установка состояния автоматического переключения режимов
    // True - автопереключение включено
    void set_state(bool is_on);
};