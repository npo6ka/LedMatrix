#pragma once

#include "snake_ai.h"

class WithFallbackAI : public SnakeAI {
    SnakeAI *primary;
    SnakeAI *fallback;

public:
    WithFallbackAI(SnakeAI *primary, SnakeAI *fallback) : primary(primary), fallback(fallback) {}
    ~WithFallbackAI() {
        delete primary;
        delete fallback;
    }

    Trend getTrend(Coord head, Coord apple, Trend vector) override {
        Trend trend = primary->getTrend(head, apple, vector);
        if (trend != Trend::none) {
            return trend;
        }
        return fallback->getTrend(head, apple, vector);
    }

    void clear() override {
        primary->clear();
        fallback->clear();
    }
};
