#pragma once

#include "effect.h"

#define START_LENGTH 4      // начальная длина змейки
#define MAX_LENGTH 80       // максимальная длина змейки
#define BRIGHTNESS 140      // ?

#define COLOR_SNAKE CRGB::Green     // основной цвет №2 для игр
#define COLOR_APPLE CRGB::Orange    // Цвет яблока на поле

boolean gameDemo = true;
uint8_t buttons = 4;   // 0 - верх, 1 - право, 2 - низ, 3 - лево, 4 - не нажата

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
    boolean apple_flag, missDelete = false;

    uint8_t buttVector[MAX_LENGTH];
    int snakeLength;
    boolean butt_flag, pizdetc;

    boolean loadingFlag = true;
    boolean gamedemo = true;

    
    uint8_t step;
    uint8_t tick;

    MyP p;

public:
    Snake() {}

    void on_init() {
        loadingFlag = true;
        step = 50;

        p.x = 1;
        p.y = 2;
    }

    void on_update() {
        snakeRoutine();
    }

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

    void snakeRoutine() {
        if (loadingFlag) {
            FastLED.clear();
            loadingFlag = false;
            newGameSnake();
        }  

        buttonsTickSnake();

        if (checkTick()) {
        // БЛОК ГЕНЕРАЦИИ ЯБЛОКА
            while (!apple_flag) {                         // пока яблоко не создано
                appleX = random(0, WIDTH);                  // взять случайные координаты
                appleY = random(0, HEIGHT);

                // проверить, не совпадает ли координата с телом змеи
                if ((long)getPixColor(appleX, appleY) == 0) {
                    apple_flag = true;                        // если не совпадает, считаем что яблоко создано
                    getPix(appleX, appleY) = COLOR_APPLE;        // и рисуем
                    FastLED.show();
                }
            }

            // запоминаем, куда повернули голову
              // 0 - право, 1 - лево, 2 - вверх, 3 - вниз
            if (vectorX > 0) buttVector[snakeLength] = 0;
            else if (vectorX < 0) buttVector[snakeLength] = 1;
            if (vectorY > 0) buttVector[snakeLength] = 2;
            else if (vectorY < 0) buttVector[snakeLength] = 3;
        
              // смещение головы змеи
            headX += vectorX;
            headY += vectorY;
        
            if (headX < 0 || headX > WIDTH - 1 || headY < 0 || headY > HEIGHT - 1) { // если вышла за границы поля
                pizdetc = true;
            }

            Serial.print(headX);
            Serial.print(" ");
            Serial.println(headY);

            if (!pizdetc) {
                // проверка на pizdetc
                if ((long)(getPixColor(headX, headY) != 0 && (long)getPixColor(headX, headY) != COLOR_APPLE)) {   // если змея врезалась во что то, но не в яблоко
                    pizdetc = true;                           // флаг на отработку
                }
        
                // БЛОК ОТРАБОТКИ ПОЕДАНИЯ ЯБЛОКА
                if (!pizdetc && (long)getPixColor(headX, headY) == (long)COLOR_APPLE) { // если попали головой в яблоко
                  apple_flag = false;                       // флаг что яблока больше нет
                  snakeLength++;                            // увеличить длину змеи
                  buttVector[snakeLength] = 4;              // запоминаем, что надо будет не стирать хвост
                }
        
                // вычисляем координату хвоста (чтобы стереть) по массиву вектора
                switch (buttVector[0]) {
                  case 0: buttX += 1;
                    break;
                  case 1: buttX -= 1;
                    break;
                  case 2: buttY += 1;
                    break;
                  case 3: buttY -= 1;
                    break;
                  case 4: missDelete = true;  // 4 значит не стирать!
                    break;
                }
        
                // смещаем весь массив векторов хвоста ВЛЕВО
                for (byte i = 0; i < snakeLength; i++) {
                  buttVector[i] = buttVector[i + 1];
                }
        
                // если змея не в процессе роста, закрасить бывший хвост чёрным
                if (!missDelete) {
                  getPix(buttX, buttY) = 0x000000;
                }
                else missDelete = false;
        
                // рисуем голову змеи в новом положении
                getPix(headX, headY) = COLOR_SNAKE;
                FastLED.show();
              }
              if (gameDemo) snakeDemo();
            }

    // если он настал
    if (pizdetc) {
      pizdetc = false;

      // ну в общем плавно моргнуть, типо змейке "больно"
      for (byte bright = 0; bright < 15; bright++) {
        FastLED.setBrightness(bright);
        for (int i = 0; i < LEDS_CNT; i++) {
          leds[i] = CRGB::Red;
        }
        FastLED.show();
        delay(10);
      }
        delay(100);
        FastLED.clear();
        FastLED.show();
        FastLED.setBrightness(BRIGHTNESS);
        //displayScore(snakeLength - START_LENGTH);
        delay(1000);
        FastLED.clear();
        FastLED.show();
        newGameSnake();                          // миша, всё ху.я, давай по новой
        }
    }   

    void snakeDemo() {
    // смещение головы змеи
    int8_t nextX = headX + vectorX;
    int8_t nextY = headY + vectorY;

    // ищем яблоко
    if (headX == appleX) {                // яблоко на одной линии по вертикали
      if (headY < appleY) buttons = 0;
      if (headY > appleY) buttons = 2;
    }
    if (headY == appleY) {                // яблоко на одной линии по горизонтали
      if (headX < appleX) buttons = 1;
      if (headX > appleX) buttons = 3;
    }

    if (getPixColor(nextX, nextY) == COLOR_SNAKE) {   // проверка на столкновение с собой
      // поворачиваем налево
      if (vectorX > 0) buttons = 0;
      if (vectorX < 0) buttons = 2;
      if (vectorY > 0) buttons = 3;
      if (vectorY < 0) buttons = 1;
      return;
    }

    if (nextX < 0 || nextX > WIDTH - 1 || nextY < 0        // проверка на столкновение со стеной
        || nextY > HEIGHT - 1) {

      // поворачиваем направо в обычном случае или налево в углу
      if (vectorX > 0) buttons = 2;
      if (vectorX > 0 && headY == 0) buttons = 0;

      if (vectorX < 0 && headY == HEIGHT - 1) buttons = 2;
      if (vectorX < 0) buttons = 0;

      if (vectorY > 0) buttons = 1;
      if (vectorY > 0 && headX == WIDTH - 1) buttons = 3;

      if (vectorY < 0 && headX == 0) buttons = 1;
      if (vectorY < 0) buttons = 3;
    }
}   

    void buttonsTickSnake() {
    if (checkButtons()) {
      if (buttons == 3) {   // кнопка нажата
        vectorX = -1;
        vectorY = 0;
        buttons = 4;
      }
      if (buttons == 1) {   // кнопка нажата
        vectorX = 1;
        vectorY = 0;
        buttons = 4;
      }
      if (buttons == 0) {   // кнопка нажата
        vectorY = 1;
        vectorX = 0;
        buttons = 4;
      }
      if (buttons == 2) {   // кнопка нажата
        vectorY = -1;
        vectorX = 0;
        buttons = 4;
      }
    }
}

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
        for (byte i = 0; i < snakeLength; i++) {
            getPix(headX - i, headY) = COLOR_SNAKE;
            //drawPixelXY(headX - i, headY, GLOBAL_COLOR_1);
            buttVector[i] = 0;
        }
        FastLED.show();
        buttX = headX - snakeLength;   // координата хвоста как голова - длина
        missDelete = false;
        apple_flag = false;
    }

};
