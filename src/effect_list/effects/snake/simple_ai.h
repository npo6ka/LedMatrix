#pragma once

#include "snake_lib.h"

class SimpleSnakeAI : public SnakeAI {
    bool canTrand(Coord coord, Trend tr) {
        auto moved_coord = coord.moveTo(tr);

        return moved_coord
            && LedMatrix.at(moved_coord.x, moved_coord.y) != COLOR_SNAKE;
    }
    
public:
    Trend getTrend(Coord head, Coord apple, Trend vector) override {
        // инициализируем кнопку. Сначала ничего не знаем, поэтому -1
        Trend trend = Trend::none;

        if (head.y == apple.y) {                // яблоко на одной линии по вертикали
            trend = head.x < apple.x ? Trend::right : Trend::left;
        }

        if (head.x == apple.x) {                // яблоко на одной линии по горизонтали
            trend = head.y < apple.y ? Trend::down : Trend::up;
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
                    trend = head.x < apple.x ? Trend::right : Trend::left;
                    break;
                case Trend::left:
                case Trend::right:
                    trend = head.y < apple.y ? Trend::down : Trend::up;
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
