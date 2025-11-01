#pragma once

#include "effects/effect.h"
#include "effects/effects_impl/snake/snake_lib.h"

#define MAX_SNAKE 10

class DribsAllSide : public Effect
{
    struct Snake {
        Coord pos;
        CRGB color;
        Trend trend;
    };

    Snake snakes[MAX_SNAKE];
    uint8_t chsv;
public:
    DribsAllSide() {}

    void on_init() {
        set_fps(60);
        chsv = 0;
        for (uint8_t i = 0; i < MAX_SNAKE; ++i) {
            snakes[i].color = 0;
        }
    }

    void remove_snake(Snake &snake) {
        snake.color = 0;
    }

    void create_snake(Snake &snake) {
        snake.color = CHSV(chsv, 255, 255);
        snake.trend = Trend(random8(4) + 1);

        switch (snake.trend) {
        case Trend::up:
            snake.pos.x = random16(LEDS_WIDTH);
            snake.pos.y = LEDS_HEIGHT - 1;
            break;
        case Trend::down:
            snake.pos.x = random16(LEDS_WIDTH);
            snake.pos.y = 0;
            break;
        case Trend::left:
            snake.pos.x = LEDS_WIDTH - 1;
            snake.pos.y = random16(LEDS_HEIGHT);
            break;
        case Trend::right:
            snake.pos.x = 0;
            snake.pos.y = random16(LEDS_HEIGHT);
            break;
        default:
            break;
        }
    }

    void on_update() {
        chsv++;

        for (uint8_t i = 0; i < MAX_SNAKE; ++i) {
            Snake &snake = snakes[i];

            if (snake.color) {
                snake.pos.move(snake.trend);

                if (!snake.pos) {
                    remove_snake(snake);
                }
            } else if (!random16(100)) { //generate new snake
                create_snake(snake);
            }
        }
    }

    void on_render() {
        for (uint8_t i = 0; i < MAX_SNAKE; ++i) {
            auto& snake = snakes[i];
            if (snake.color) {
                LedMatrix.at(snake.pos.x, snake.pos.y) = snake.color;
            }
        }

        LedMatrix.fader(5);
    }
};
