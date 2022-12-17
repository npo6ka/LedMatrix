#pragma once

#include "effect_list/libs/queue.h"
#include "effect_list/effect.h"
#include "simple_ai.h"
#include "a_star_ai.h"
#include "with_fallback_ai.h"

class Snake : public Effect
{
    Queue<Trend, MAX_SNAKE_LENGTH> snake;
    Coord head, butt, apple;
    Trend button, vector;
    bool apple_flag, end_game;
    uint8_t aiType = 3;
    SnakeAI *ai;

    uint8_t tick, step = 1;

        // Если нужно, можно вызвать этот метод. Выводит отладочную информацию в терминал
    void debug() {
        out("vector %d\n", vector);
        out("head.x = %d head.y = %d\n", head.x, head.y);
        out("butt.x = %d butt.y = %d\n", butt.x, butt.y);
        out("apple.x = %d apple.y = %d\n\n", apple.x, apple.y);
        snake.debug();
        out("---\n");
    }

    // Метод выполняется каждый тик. Тут вся логика
    void snakeRoutine() {
        // проверяем на наличие яблока
        genApple();
        getPix(apple.x, apple.y) = COLOR_APPLE;

        // Тут мозг змеки

        if (ai) {
            button = ai->getTrend(head, apple, vector);
        }

        // Определяем куда идти змейке по нажатой кнопке
        buttonsTickSnake();

        // смещение головы змеи
        head.move(vector);
        // запоминаем, куда была повернута голова
        snake.push(vector);

        if (!head) { // проверяем что змека не вышла за границы поля
            end_game = true;
        } else if (snake.size() >= MAX_SNAKE_LENGTH) { // проверяем что змейка не прывисила максимальную длину
            end_game = true;
        } else if (getPix(head.x, head.y) && head != apple) { // проверяем что змейка врезалась во что то, но не в яблоко
            end_game = true;
        }

        if (!end_game) {
            bool is_feeding = false;

            if (head == apple) {    // если попали головой в яблоко
                apple_flag = false; // флаг что яблока больше нет
                is_feeding = true;
            }

            if (snake.size() < START_LENGTH)
                is_feeding = true;

            // если змея не в процессе роста, закрасить бывший хвост чёрным
            if (!is_feeding) {
                getPix(butt.x, butt.y) = 0x000000;
                butt.move(snake.front());
                snake.popFront();
            }

            // рисуем голову змеи в новом положении
            getPix(head.x, head.y) = COLOR_SNAKE;
        } else { // end_game == true
            // ну в общем плавно моргнуть, типо змейке "больно"
            CRGB *leds = getLeds();

            for (uint8_t bright = 0; bright < 16; ++bright) {
                for (int i = 0; i < LEDS_CNT; i++) {
                    leds[i] = CRGB::Red / 15 * bright;
                }

                FastLED.show();
                delay(100);
            }

            out("End game: Score: %d\n", snake.size());

            //delay(100);
            //FastLED.clear();
            //FastLED.show();
            //displayScore(snakeLength - START_LENGTH);
            //delay(100);
            ai->clear();
            newGameSnake();      // миша, всё ху.я, давай по новой
        }
    }

    void buttonsTickSnake() {
        if (button != Trend::none) {
            vector = button;
            button = Trend::none;
        }
    }

    void setApple(uint8_t x, uint8_t y) {
        apple = {x, y};
        apple_flag = true;
        getPix(apple.x, apple.y) = COLOR_APPLE;
    }

    // Генерация яблока, если его нету на поле
    void genApple() {
        if (apple_flag) {
            return;
        }

        uint16_t cnt = 0, pos = random16(LEDS_CNT - snake.size() - 1);
        // считаем пустые клетки и заодно проверяем равна ли
        // клетка тому что зарандомили
        for (uint8_t i = 0; i < HEIGHT; i++) {
            for (uint8_t j = 0; j < WIDTH; j++) {
                if (!getPix(i, j)) {
                    if (cnt == pos) {
                        setApple(i, j);
                        return;
                    }
                    cnt++;
                }
            }
        }
    }


    // Новая игра. Генерация всего с самого начала
    void newGameSnake() {
        FastLED.clear();

        vector = Trend::up;  // начальный вектор движения задаётся вот здесь
        button = Trend::none;

        // длина из настроек, начинаем в середине экрана, бла-бла-бла
        head = {HEIGHT / 2 - 1, WIDTH / 2 - 1};
        butt = head;
        getPix(head.x, head.y) = COLOR_SNAKE; // устанавливаем первый пиксель без добавления в очередь
        snake.clear();

        apple_flag = false;
        end_game = false;
    }

    void initAi() {
        switch (aiType) {
        case 1:
            ai = new SimpleSnakeAI();
            break;
        case 2:
            ai = new AStarSnakeAI();
            break;
        case 3:
            ai = new WithFallbackAI(new AStarSnakeAI(), new SimpleSnakeAI());
            break;
        }
    }

public:
    void on_init() override {
        set_fps(20);
        tick = 0;
        button = Trend::none;

        newGameSnake();
        initAi();
    }

    ~Snake() {
        if (ai) {
            delete ai;
        }
    }

    void on_update() override {
        tick = (tick + 1) % step;
        if (!tick) {
            snakeRoutine();
        }
    }
};