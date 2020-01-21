#pragma once

#include "effect.h"

#define START_LENGTH 4      // начальная длина змейки
#define MAX_LENGTH 80       // максимальная длина змейки
#define BRIGHTNESS 140      // ?

#define COLOR_SNAKE CRGB::Green     // основной цвет №2 для игр
#define COLOR_APPLE CRGB::Orange    // Цвет яблока на поле

bool gameDemo = true;
uint8_t buttons = 4;   // 0 - верх, 1 - право, 2 - низ, 3 - лево, 4 - не нажата

// точка на матрице
struct MyP
{
    uint8_t x;
    uint8_t y;
};


class Snake : public Effect
{
    uint8_t inc_val[LEDS_CNT];
    uint8_t vectorX, vectorY;
    uint8_t headX, headY, buttX, buttY;
    uint8_t appleX, appleY;
    bool apple_flag, missDelete = false;

    uint8_t buttVector[MAX_LENGTH];
    int snakeLength;
    bool butt_flag, pizdetc;

    bool loadingFlag = true;
    bool gamedemo = true;

    
    uint8_t step;
    uint8_t tick;

public:
    Snake() {}

    void on_init() {
        loadingFlag = true;
        step = 30;
    }

    void on_update() {
        snakeRoutine();
    }

    // Меняем параметры местами, чтобы было привычнее работать.
    CRGB &getPix_(int x, int y) {
        return getPix(y, x);
    }
    
    uint32_t getPixColor_(int x, int y) {
        return getPixColor(y, x);
    }

    // Проверить, пора ли двигаться змейке. По сути тут задаётся скорость змейки
    bool checkTick(){
        if (tick >= step) {
            tick = 0;
            return true;
        } else {
            tick++;
        }
        return false;
    }
    
    uint8_t checkButtons(){
      // тут заглушка
      return buttons;
    }

    // Метод выполняется каждый тик. Тут вся логика
    void snakeRoutine() {
        if (loadingFlag) {
            FastLED.clear();
            loadingFlag = false;
            newGameSnake();
        }  
        
        // Если время не пришло, то ничего не делаем
        if (!checkTick()) 
            return;
        
        // проверяем на наличие яблока
        apple_flag = false;
        genApple();

    }

    // Генерация яблока, если его нету на поле
    void genApple(){
        while (!apple_flag) {                         // пока яблоко не создано
            appleX = random(0, WIDTH);                  // взять случайные координаты
            appleY = random(0, HEIGHT);

            // проверить, не совпадает ли координата с телом змеи
            if ((long)getPixColor_(appleX, appleY) == 0) {
                apple_flag = true;                        // если не совпадает, считаем что яблоко создано
                getPix(appleX, appleY) = COLOR_APPLE;        // и рисуем
                //FastLED.show();
            }else{
                out("long)getPixColor_(appleX, appleY) != 0");
            }
        }
    }

    // Новая игра. Генерация всего с самого начала
    void newGameSnake() {
        FastLED.clear();
        // свежее зерно для генератора случайных чисел
        randomSeed(millis());

        // длина из настроек, начинаем в середине экрана, бла-бла-бла
        snakeLength = START_LENGTH;
        headX = WIDTH / 2;
        headY = HEIGHT / 2;
        buttY = headY;

        vectorX = 1;  // начальный вектор движения задаётся вот здесь
        vectorY = 0;
        buttons = 4;

        // первоначальная отрисовка змейки и забивка массива векторов для хвоста
        for (uint8_t i = 0; i < snakeLength; i++) {
            getPix_(headX - i, headY) = COLOR_SNAKE;
            buttVector[i] = 0;
        }
        FastLED.show();
        buttX = headX - snakeLength;   // координата хвоста как голова - длина
        missDelete = false;
        apple_flag = false;
    }

};