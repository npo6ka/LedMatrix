#pragma once

#include "snake_lib.h"

class SimpleSnakeAI : public SnakeAI {
    bool canTrand(Coord coord, Trend tr) {
        switch (tr) {
        case Trend::up:
            return coord.x - 1 >= 0 && getPix(coord.x - 1, coord.y) != COLOR_SNAKE;
        case Trend::right:
            return coord.y + 1 < WIDTH && getPix(coord.x, coord.y + 1) != COLOR_SNAKE;
        case Trend::down:
            return coord.x + 1 < HEIGHT && getPix(coord.x + 1, coord.y) != COLOR_SNAKE;
        case Trend::left:
            return coord.y - 1 >= 0 && getPix(coord.x, coord.y - 1) != COLOR_SNAKE;
        case Trend::none:
            return false;
        }
        return false;
    }
public:
    Trend getTrend(Coord head, Coord apple, Trend vector) override {
        // инициализируем кнопку. Сначала ничего не знаем, поэтому -1
        Trend trend = Trend::none;

        if (head.x == apple.x) {                // яблоко на одной линии по вертикали
            trend = head.y < apple.y ? Trend::right : Trend::left;
        }

        if (head.y == apple.y) {                // яблоко на одной линии по горизонтали
            trend = head.x < apple.x ? Trend::down : Trend::up;
        }

        // Если не поняли куда идти или туда идти нельзя, то нужно придумать другой ход.
        if (!canTrand(head, trend)) {
            // Если пытались пойти против движения то сбрасываем направление и ищем дургое
            trend = Trend::none;

            // Пытаемся продолжить движение в том же направлении
            if (canTrand(head, vector)) {
                trend = vector;
            } else {
                // пытаемся повернуться в сторону яблока
                switch (vector) {
                case Trend::down:
                case Trend::up:
                    trend = head.y < apple.y ? Trend::right : Trend::left;
                    break;
                case Trend::left:
                case Trend::right:
                    trend = head.x < apple.x ? Trend::down : Trend::up;
                    break;
                default:
                    trend = Trend::none;
                }

                if (!canTrand(head, trend)) {
                    // Если нету направления, то берем первое доступное
                    for (auto itrend : {Trend::down, Trend::left, Trend::up, Trend::right}) {
                        if (canTrand(head, itrend)) {
                            trend = itrend;
                            break;
                        }
                    }
                }
            }
        }

        return trend;
    }
};
