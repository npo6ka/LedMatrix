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

    uint8_t tick, step = 1; //ход раз в step кадров: меньше — быстрее змейка
    uint8_t endAnimBright = 0;
    uint32_t endAnimNextMs = 0;

    // Одномерное поле (лента 1xN или Nx1). Тело хранится позициями вдоль ленты,
    // а не поворотами: в одной строке змейка ходит только вперёд-назад.
    bool line_mode = false;
    index_t line_body[MAX_SNAKE_LENGTH];
    uint16_t line_start = 0, line_len = 0, line_target = 0;
    int8_t line_dir = 1;
    index_t line_apple = 0;
    bool line_apple_flag = false;

    static bool isLineMatrix() {
        return LedMatrix.width() <= 1 || LedMatrix.height() <= 1;
    }

    index_t lineSize() const {
        const size_t size = LedMatrix.size();
        return size > MAX_SNAKE_LENGTH ? (index_t)MAX_SNAKE_LENGTH : (index_t)size;
    }

    index_t lineAt(uint16_t i) const {
        return line_body[(line_start + i) % MAX_SNAKE_LENGTH];
    }

    index_t lineHead() const {
        return lineAt(line_len - 1);
    }

    void linePush(index_t pos) {
        line_body[(line_start + line_len) % MAX_SNAKE_LENGTH] = pos;
        if (line_len < MAX_SNAKE_LENGTH) {
            ++line_len;
        } else {
            line_start = (line_start + 1) % MAX_SNAKE_LENGTH;
        }
    }

    void linePopTail() {
        if (line_len) {
            line_start = (line_start + 1) % MAX_SNAKE_LENGTH;
            --line_len;
        }
    }

    bool lineOccupied(index_t pos) const {
        for (uint16_t i = 0; i < line_len; ++i) {
            if (lineAt(i) == pos) {
                return true;
            }
        }
        return false;
    }

    void genAppleLine() {
        if (line_apple_flag) {
            return;
        }

        const index_t len = lineSize();
        for (index_t pos = 0; pos < len; ++pos) {
            const index_t candidate = (index_t)((random16(len) + pos) % len);
            if (!lineOccupied(candidate)) {
                line_apple = candidate;
                line_apple_flag = true;
                return;
            }
        }
    }

    void renderLine() {
        const index_t len = lineSize();
        for (index_t i = 0; i < len; ++i) {
            LedMatrix.atLinear(i) = CRGB(0, 0, 0);
        }
        for (uint16_t i = 0; i < line_len; ++i) {
            LedMatrix.atLinear(lineAt(i)) = COLOR_SNAKE;
        }
        if (line_apple_flag) {
            LedMatrix.atLinear(line_apple) = COLOR_APPLE;
        }
    }

    void snakeRoutineLine() {
        genAppleLine();

        const index_t len = lineSize();
        const index_t head = lineHead();

        // разворот на месте разрешён: иначе до яблока за спиной не добраться
        if (line_apple_flag) {
            if (line_apple > head) {
                line_dir = 1;
            } else if (line_apple < head) {
                line_dir = -1;
            }
        }

        int32_t next = (int32_t)head + line_dir;
        if (next < 0 || next >= (int32_t)len) {
            line_dir = -line_dir;
            next = (int32_t)head + line_dir;
        }

        linePush((index_t)next);

        if (line_apple_flag && (index_t)next == line_apple) {
            line_apple_flag = false;
            if (line_target < len) {
                ++line_target;
            }
        }

        while (line_len > line_target) {
            linePopTail();
        }

        renderLine();

        if (line_target >= len) {
            end_game = true;
            startEndGameAnim();
        }
    }

    void newGameLine() {
        FastLED.clear();

        const index_t len = lineSize();
        line_start = 0;
        line_len = 0;
        line_target = START_LENGTH < len ? START_LENGTH : 1;
        line_dir = 1;
        line_apple_flag = false;
        linePush((index_t)(len / 2));

        end_game = false;
        endAnimBright = 0;
        startTime = millis();
    }

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
        if (line_mode) {
            newGameLine();
            return;
        }

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
        set_fps(60);
        tick = 0;
        button = Trend::none;
        ai = nullptr;
        line_mode = isLineMatrix();
        // на ленте до яблока десятки диодов, поэтому шаг каждый кадр
        step = line_mode ? 1 : 4;

        if (!canPlay()) {
            return;
        }

        // на ленте поиск пути не нужен: направление определяется знаком до яблока
        if (!line_mode) {
            ai = make_ai();
        }
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
            if (line_mode) {
                snakeRoutineLine();
            } else {
                snakeRoutine();
            }
        }
    }

    virtual bool is_end() const override {
        //if (!canPlay()) {
            return true;
        //}
        return millis() - startTime < 5000; // если с момента старта прошло больше 5 секунд, то режим не переключится
    }
};
