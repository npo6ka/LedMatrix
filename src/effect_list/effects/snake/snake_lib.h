#pragma once

#include "stdint.h"

#define START_LENGTH 4                      // начальная длина змейки
#define MAX_SNAKE_LENGTH LEDS_HEIGHT * LEDS_WIDTH     // максимальная длина змейки
static_assert(LEDS_HEIGHT / 2 >= START_LENGTH, "Low LEDS_HEIGHT size for starting snake mode");

#define COLOR_SNAKE CRGB(CRGB::Green)       // Цвет змейки
#define COLOR_APPLE CRGB(CRGB::Orange)      // Цвет яблока на поле

enum class Trend : uint8_t {
    none = 0,
    up,    // 1
    right, // 2
    down,  // 3
    left,  // 4
};

inline Trend invertTrend(Trend trend) {
    switch (trend) {
    case Trend::up:
        return Trend::down;
    case Trend::right:
        return Trend::left;
    case Trend::down:
        return Trend::up;
    case Trend::left:
        return Trend::right;
    case Trend::none:
        return Trend::none;
    }
    return Trend::none;
}

struct Coord {
    uint8_t x;
    uint8_t y;

    void move(Trend tr) {
        switch (tr) {
        case Trend::up:
            y--;
            break;
        case Trend::right:
            x++;
            break;
        case Trend::down:
            y++;
            break;
        case Trend::left:
            x--;
            break;
        case Trend::none:
            break;
        }
    }

    Coord moveTo(Trend tr) const {
        Coord coord = *this;
        coord.move(tr);
        return coord;
    }

    operator bool() const {
        return x < LEDS_WIDTH && y < LEDS_HEIGHT;
    }

    bool operator==(Coord a) const {
        return x == a.x && y == a.y;
    }

    bool operator!=(Coord a) const {
        return !(*this == a);
    }

};

class SnakeAI {
public:
    virtual ~SnakeAI() = default;
    virtual Trend getTrend(Coord head, Coord apple, Trend vector) = 0;
    virtual void clear() {};
};
