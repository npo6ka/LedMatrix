#pragma once

#include "stdint.h"
#include "configs/constants.h"

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
