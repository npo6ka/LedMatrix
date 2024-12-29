#pragma once

#include "libs/led_matrix.h"

class Effect
{
    uint8_t _fps = 60;
    bool _is_end = true;

public:
    Effect() = default;
    virtual ~Effect() = default;

    /* Инициализация режима, установка начальный значений.
    *  Выполняется единожды, при выборе нового режима.
    * */
    virtual void on_init() {}

    /* on_update обязательно нужно описать в наследукмом классе.
    *  Обновление логики эффекта.
    *  Часть цикла работы режима.
    * */
    virtual void on_update() = 0;

    /* on_render может быть переинициализирован в наследукмом классе.
    *  Обновление графики эффекта.
    *  Часть цикла работы режима.
    * */
    virtual void on_render() {}

    /* on_clear может быть переинициализирован в наследукмом классе.
    *  Выполняется единожды, при выборе нового режима, перед вызовом on_init.
    *  Действия с состоянием матрицы оставшейся после предыдущего режима.
    *  По умолчанию всё стрирается.
    * */
    virtual void on_clear() {
        FastLED.clear();
    }

    /* set_fps устанавливает максимальный фпс для режима. Может регулировать скорость режима,
    *  однако скорость режима лучше регулировать кодом внутри режима.
    * */
    void set_fps(uint8_t val) {
        _fps = val;
    }

    /* get_fps возвращает максимально допустимый фпс для режима. ФПС может регулировать скорость режима,
    *  однако скорость режима лучше регулировать кодом внутри режима.
    *  По умолчанию 60.
    * */
    uint8_t get_fps() {
        return _fps;
    }

    /* is_end проверерят закончился ли режим, можно ли его переключить на следующий
    *  По умолчанию значение true, режим можно переключить всегда.
    *  Если не хочется чтобы автопереключение сменило режим в середине какого то процесса режима
    *  нужно установить флаг в false. Нужно обязательно следить чтобы флаг устанавливался в true
    *  и был таковым на протяжении 2 и более секунд.
    */
    bool is_end() {
        return _is_end;
    }
};
