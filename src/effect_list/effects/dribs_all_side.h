#pragma once

#include "effect_list/effect.h"

#define MAX_SNAKE 10

class DribsAllSide : public Effect
{
    enum Trend {
        up = 1,
        down,
        left,
        right
    };

    struct Snake {
        uint16_t x;
        uint16_t y;
        CRGB cl;
        Trend trend; // 1 up 2 down 3 left 4 right
    };

    Snake snakes[MAX_SNAKE];
    uint8_t chsv;
public:
    DribsAllSide() {}

    void on_init() {
        set_fps(60);
        chsv = 0;
        for (uint8_t i = 0; i < MAX_SNAKE; ++i) {
            snakes[i].cl = 0;
        }
    }

    void remove_snake(Snake &snake) {
        snake.cl = 0;
    }

    void create_snake(Snake &snake) {
        snake.cl = CHSV(chsv, 255, 255);
        snake.trend = Trend(random8(4) + 1);
    }

    void on_update() {
        chsv++;

        for (uint8_t i = 0; i < MAX_SNAKE; ++i) {
            Snake &cur_snake = snakes[i];
            if (cur_snake.cl) {
                switch (cur_snake.trend)
                {
                case Trend::up:
                    if (--cur_snake.y >= WIDTH) {
                        remove_snake(cur_snake);
                    }
                    break;
                case Trend::down:
                    if (++cur_snake.y >= WIDTH) {
                        remove_snake(cur_snake);
                    }
                    break;
                case Trend::left:
                    if (--cur_snake.x >= HEIGHT) {
                        remove_snake(cur_snake);
                    }
                    break;
                case Trend::right:
                    if (++cur_snake.x >= HEIGHT) {
                        remove_snake(cur_snake);
                    }
                    break;
                }
            } else {
                //generate new snake
                if (!random16(100)) {
                    create_snake(cur_snake);

                    switch (cur_snake.trend)
                    {
                    case Trend::up:
                        cur_snake.x = random16(HEIGHT - 1);
                        cur_snake.y = WIDTH - 1;
                        break;
                    case Trend::down:
                        cur_snake.x = random16(HEIGHT - 1);
                        cur_snake.y = 0;
                        break;
                    case Trend::left:
                        cur_snake.x = HEIGHT - 1;
                        cur_snake.y = random16(WIDTH - 1);
                        break;
                    case Trend::right:
                        cur_snake.x = 0;
                        cur_snake.y = random16(WIDTH - 1);
                        break;
                    }
                }
            }
        }
    }

    void on_render() {
        for (uint8_t i = 0; i < MAX_SNAKE; ++i) {
            Snake cur_snake = snakes[i];
            if (cur_snake.cl) {
                getPix(cur_snake.x, cur_snake.y) = cur_snake.cl;
            }
        }

        fader(5);
    }
};
