#pragma once

#include "libs/queue.h"
#include "effects/effect.h"
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

    uint8_t tick, step = 3;

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
        LedMatrix.at(apple.x, apple.y) = COLOR_APPLE;

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
        } else if (LedMatrix.at(head.x, head.y) && head != apple) { // проверяем что змейка врезалась во что то, но не в яблоко
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
                LedMatrix.at(butt.x, butt.y) = 0x000000;
                butt.move(snake.front());
                snake.popFront();
            }

            // рисуем голову змеи в новом положении
            LedMatrix.at(head.x, head.y) = COLOR_SNAKE;
        } else { // end_game == true
            // ну в общем плавно моргнуть, типо змейке "больно"

            for (uint8_t bright = 0; bright < 16; ++bright) {
                for (size_t i = 0; i < LedMatrix.size(); ++i) {
                    LedMatrix.at(i) = CRGB::Red / 15 * bright;
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
        LedMatrix.at(apple.x, apple.y) = COLOR_APPLE;
    }

    // Генерация яблока, если его нету на поле
    void genApple() {
        if (apple_flag) {
            return;
        }

        uint16_t cnt = 0, pos = random16(LedMatrix.size() - snake.size() - 1);
        // считаем пустые клетки и заодно проверяем равна ли
        // клетка тому что зарандомили
        for (auto x : LedMatrix.rangeX()) {
            for (auto y : LedMatrix.rangeY()) {
                if (!LedMatrix.at(x, y)) {
                    if (cnt == pos) {
                        setApple(x, y);
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
        head = {LEDS_WIDTH / 2 - 1, LEDS_HEIGHT / 2 - 1};
        butt = head;
        LedMatrix.at(head.x, head.y) = COLOR_SNAKE; // устанавливаем первый пиксель без добавления в очередь
        snake.clear();

        apple_flag = false;
        end_game = false;
    }

    SnakeAI* make_ai() {
        switch (aiType) {
        case 1:
            return new SimpleSnakeAI();
        case 2:
            return new AStarSnakeAI();
        case 3:
            return new WithFallbackAI(new AStarSnakeAI(), new SimpleSnakeAI());
        default:
            return nullptr;
        }
    }

public:
    void on_init() override {
        set_fps(40);
        tick = 0;
        button = Trend::none;
        ai = make_ai();

        newGameSnake();
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
