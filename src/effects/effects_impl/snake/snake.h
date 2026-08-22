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
    SnakeAI *ai = nullptr;
    uint64_t startTime;

    uint8_t tick, step = 3;
    uint8_t endAnimBright = 0;
    uint32_t endAnimNextMs = 0;

        // Если нужно, можно вызвать этот метод. Выводит отладочную информацию в терминал
    void debug() {
        logInfo("vector %d\n", vector);
        logInfo("head.x = %d head.y = %d\n", head.x, head.y);
        logInfo("butt.x = %d butt.y = %d\n", butt.x, butt.y);
        logInfo("apple.x = %d apple.y = %d\n\n", apple.x, apple.y);
        snake.debug();
        logInfo("---\n");
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
        } else {
            startEndGameAnim();
        }
    }

    void buttonsTickSnake() {
        if (button != Trend::none) {
            vector = button;
            button = Trend::none;
        }
    }

    void setApple(index_t x, index_t y) {
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


    void startEndGameAnim() {
        endAnimBright = 0;
        endAnimNextMs = millis();
    }

    bool tickEndGameAnim() {
        if (!end_game) {
            return false;
        }
        if (millis() < endAnimNextMs) {
            return true;
        }
        endAnimNextMs = millis() + 100;

        if (endAnimBright < 16) {
            const CRGB color = CRGB::Red / 15 * endAnimBright;
            for (size_t i = 0; i < LedMatrix.size(); ++i) {
                LedMatrix.at(i) = color;
            }
            ++endAnimBright;
            return true;
        }

        logInfo("End game: Score: %d\n", snake.size());
        if (ai) {
            ai->clear();
        }
        newGameSnake();
        return false;
    }

    // Новая игра. Генерация всего с самого начала
    void newGameSnake() {
        FastLED.clear();

        vector = Trend::up;  // начальный вектор движения задаётся вот здесь
        button = Trend::none;

        // длина из настроек, начинаем в середине экрана
        const index_t w = LedMatrix.width();
        const index_t h = LedMatrix.height();
        head = {
            w > 1 ? static_cast<index_t>(w / 2 - 1) : index_t{0},
            h > 1 ? static_cast<index_t>(h / 2 - 1) : index_t{0}
        };
        butt = head;
        LedMatrix.at(head.x, head.y) = COLOR_SNAKE; // устанавливаем первый пиксель без добавления в очередь
        snake.clear();

        apple_flag = false;
        end_game = false;
        endAnimBright = 0;

        startTime = millis();
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

    static bool canPlay() {
        return LEDS_SIZE > START_LENGTH;
    }

public:
    virtual void on_init() override {
        set_fps(40);
        tick = 0;
        button = Trend::none;
        ai = nullptr;

        if (!canPlay()) {
            return;
        }

        ai = make_ai();
        newGameSnake();
    }

    ~Snake() {
        if (ai) {
            delete ai;
        }
    }

    virtual void on_update() override {
        if (!canPlay()) {
            return;
        }

        if (tickEndGameAnim()) {
            return;
        }

        tick = (tick + 1) % step;
        if (!tick) {
            snakeRoutine();
        }
    }

    virtual bool is_end() const override {
        if (!canPlay()) {
            return true;
        }
        return millis() - startTime < 5000; // если с момента старта прошло больше 5 секунд, то режим не переключится
    }
};
