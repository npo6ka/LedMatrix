#pragma once

#include "effect_list/libs/queue.h"
#include "effect_list/effect.h"

#define START_LENGTH 4                      // начальная длина змейки
#define MAX_SNAKE_LENGTH HEIGHT * WIDTH     // максимальная длина змейки
#if (HEIGHT / 2 < START_LENGTH)
#   error Low HEIGHT size for starting snake mode
#endif

#define COLOR_SNAKE CRGB(CRGB::Green)       // Цвет змейки
#define COLOR_APPLE CRGB(CRGB::Orange)      // Цвет яблока на поле

class Snake : public Effect
{
    enum class Trend : uint8_t {
        none = 0,
        up,
        right,
        down,
        left,
    };

    struct Coord {
        uint8_t x;
        uint8_t y;

        void move(Trend tr) {
            switch (tr) {
            case Trend::up:
                x--;
                break;
            case Trend::right:
                y++;
                break;
            case Trend::down:
                x++;
                break;
            case Trend::left:
                y--;
                break;
            case Trend::none:
                break;
            }
        }

        void move_opposite(Trend tr) {
            switch (tr) {
            case Trend::up:
                x++;
                break;
            case Trend::right:
                y--;
                break;
            case Trend::down:
                x--;
                break;
            case Trend::left:
                y++;
                break;
            case Trend::none:
                break;
            }
        }

        bool is_valid() {
            return x < HEIGHT && y < WIDTH;
        }
    };

    Queue<Trend, MAX_SNAKE_LENGTH> snake;
    Coord head, butt, apple;
    Trend button, vector;
    bool apple_flag, end_game, gameDemo;

    uint8_t tick, step = 3;

public:
    void on_init() {
        set_fps(20);
        gameDemo = true;
        tick = 0;
        button = Trend::none;

        newGameSnake();
    }

    void on_update() {
        tick = (tick + 1) % step;
        if (!tick) {
            snakeRoutine();
        }
    }

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

        // Тут мозг змеки
        if (gameDemo) snakeDemo();

        // Определяем куда идти змейке по нажатой кнопке
        buttonsTickSnake();

        // смещение головы змеи
        head.move(vector);
        // запоминаем, куда была повернута голова
        snake.push(vector);

        if (!head.is_valid()) { // проверяем что змека не вышла за границы поля
            end_game = true;
        } else if (snake.size() + 1 >= MAX_SNAKE_LENGTH) { // проверяем что змейка не прывисила максимальную длину
            end_game = true;
        } else if ((getPix(head.x, head.y) && getPix(head.x, head.y) != CRGB(COLOR_APPLE))) { // проверяем что змейка врезалась во что то, но не в яблоко
            end_game = true;
        }

        if (!end_game) {
            bool is_feeding = false;

            if (getPix(head.x, head.y) == CRGB(COLOR_APPLE)) { // если попали головой в яблоко
                apple_flag = false;                            // флаг что яблока больше нет
                is_feeding = true;
                genApple();
            }

            // если змея не в процессе роста, закрасить бывший хвост чёрным
            if (!is_feeding) {
                getPix(butt.x, butt.y) = 0x000000;
                butt.move(snake.front());
                snake.pop();
            }

            // рисуем голову змеи в новом положении
            getPix(head.x, head.y) = COLOR_SNAKE;
        } else { // end_game == true
            // ну в общем плавно моргнуть, типо змейке "больно"
            CRGB *leds = getLeds();

            for (byte bright = 0; bright < 15; bright++) {
                for (int i = 0; i < LEDS_CNT; i++) {
                    leds[i] = CRGB::Red;
                }

                FastLED.show();
                delay(50);
            }

            //delay(100);
            //FastLED.clear();
            //FastLED.show();
            //displayScore(snakeLength - START_LENGTH);
            //delay(100);

            newGameSnake();      // миша, всё ху.я, давай по новой
        }
    }

    void buttonsTickSnake() {
        if (button != Trend::none) {
            vector = button;
            button = Trend::none;
        }
    }

    bool canTrand(Coord coord, Trend tr) {
        switch (tr) {
        case Trend::up:
            return coord.x - 1 >= 0 && getPix(coord.x - 1, coord.y) != COLOR_SNAKE;
        case Trend::right:
            return coord.y + 1 < WIDTH && getPix(coord.x, coord.y + 1) != COLOR_SNAKE;
        case Trend::down:
            return coord.x + 1 < HEIGHT && getPix(coord.x + 1, coord.y) != COLOR_SNAKE;
        case Trend::left:
            return coord.y - 1 >= 0 && getPix(coord.x, coord.y - 1) != COLOR_SNAKE;
        case Trend::none:
            return false;
        }
        return false;
    }

    void snakeDemo() {
        // инициализируем кнопку. Сначала ничего не знаем, поэтому -1
        Trend tmp_button = Trend::none;

        if (head.x == apple.x) {                // яблоко на одной линии по вертикали
            tmp_button = head.y < apple.y ? Trend::right : Trend::left;
        }

        if (head.y == apple.y) {                // яблоко на одной линии по горизонтали
            tmp_button = head.x < apple.x ? Trend::down : Trend::up;
        }

        // // Если не поняли куда идти или туда идти нельзя, то нужно придумать другой ход.
        if (!canTrand(head, tmp_button)) {
            // Если пытались пойти против движения то сбрасываем направление и ищем дургое
            tmp_button = Trend::none;

            // Пытаемся продолжить движение в том же направлении
            if (canTrand(head, vector)) {
                tmp_button = vector;
            } else {
                // Если нету направления, то берем первое доступное
                for (auto trend : {Trend::down, Trend::left, Trend::up, Trend::right}) {
                    if (canTrand(head, trend)) {
                        tmp_button = trend;
                        break;
                    }
                }
            }
        }

        if (tmp_button != Trend::none) {
            button = tmp_button;
        }
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
                        apple = {i, j};
                        apple_flag = true;
                        getPix(apple.x, apple.y) = COLOR_APPLE;
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
        butt = {uint8_t(head.x + uint8_t(START_LENGTH) - 1), head.y}; // координата хвоста как голова - длина
        getPix(head.x, head.y) = COLOR_SNAKE; // устанавливаем первый пиксель без добавления в очередь
        snake.clear();

        // первоначальная отрисовка змейки и забивка массива векторов для хвоста
        for (uint8_t i = 1; i < START_LENGTH; i++) {
            getPix(head.x + i, head.y) = COLOR_SNAKE;
            snake.push(Trend::up);
        }

        apple_flag = false;
        end_game = false;
    }

};