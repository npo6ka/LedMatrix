#pragma once

#include "stdint.h"
#include "libs/coord.h"

class SnakeAI {
public:
    virtual ~SnakeAI() = default;
    virtual Trend getTrend(Coord head, Coord apple, Trend vector) = 0;
    virtual void clear() {};
};
