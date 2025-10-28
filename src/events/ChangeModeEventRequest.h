#pragma once

#include "stdint.h"

struct ChangeModeEventRequest {
    enum class Type {
        None,
        Set,
        Next,
        Previous
    };
    Type type;
    uint16_t modNum; // на какой конкретно режим сменить. Учитывается только при Type::Set
    int32_t modNumOffset; // сдвиг относительно текущего мода. Учитывается только при Type::Next и Type::Previous
    bool hardReset;

    ChangeModeEventRequest() : type(Type::None), modNum(0), modNumOffset(0), hardReset(false) {}
    ChangeModeEventRequest(Type t, bool h, uint16_t modNum = 0, int32_t modNumOffset = 0) :
        type(t),
        modNum(modNum),
        modNumOffset(modNumOffset),
        hardReset(h)
    {
        if (t == Type::Set) {
            modNumOffset = 0;
        } else if (t == Type::Next || t == Type::Previous) {
            modNum = 0;
        }
    }

    // Оператор сложения для объединения запросов
    ChangeModeEventRequest operator+(const ChangeModeEventRequest& other) const {
        // Если текущий запрос - None, возвращаем другой
        if (type == Type::None) {
            return other;
        }

        // Если другой запрос - None, возвращаем текущий
        if (other.type == Type::None) {
            return *this;
        }

        // Приоритет: Set > Next/Previous
        if (other.type == Type::Set) {
            return other;
        }

        // Если оба Next или Previous, объединяем offset'ы
        if (type == other.type) {
            return ChangeModeEventRequest(
                type,
                hardReset || other.hardReset,
                modNum,
                modNumOffset + other.modNumOffset
            );
        }

        // Если разные типы Next/Previous, рассчитываем итоговый offset
        int32_t finalOffset = 0;
        Type finalType = Type::Next; // по умолчанию Next

        if (type == Type::Next && other.type == Type::Previous) {
            finalOffset = modNumOffset - other.modNumOffset;
        } else if (type == Type::Previous && other.type == Type::Next) {
            finalOffset = other.modNumOffset - modNumOffset;
        }

        // Определяем итоговый тип на основе знака offset
        if (finalOffset > 0) {
            finalType = Type::Next;
        } else if (finalOffset < 0) {
            finalType = Type::Previous;
            finalOffset = -finalOffset; // делаем положительным для Previous
        } else {
            finalType = Type::None; // если offset = 0, то никакого изменения
        }

        return ChangeModeEventRequest(
            finalType,
            hardReset || other.hardReset,
            modNum,
            finalOffset
        );
    }
};