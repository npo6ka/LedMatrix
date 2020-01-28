#pragma once

#include "effect.h"

#define START_LENGTH 4      // начальная длина змейки
#define MAX_LENGTH 80       // максимальная длина змейки

#define COLOR_SNAKE CRGB::Green     // Цыет змейки
#define COLOR_APPLE CRGB::Orange    // Цвет яблока на поле

bool gameDemo = true;
uint8_t buttons = 4;   // 0 - верх, 1 - право, 2 - низ, 3 - лево, 4 - не нажата

class Snake : public Effect
{
    int8_t vectorX, vectorY;
    int8_t headX, headY, buttX, buttY;
    int8_t appleX, appleY;
    bool apple_flag, missDelete = false;

    int8_t buttVector[MAX_LENGTH];
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
        step = 3;
    }

    void on_update() {
        snakeRoutine();
    }

    // Меняем параметры местами, чтобы было привычнее работать.
    CRGB &getPix_(int x, int y) {
        if (x > WIDTH - 1 || y > HEIGHT - 1)
            return getPix(0, 0);
        return getPix(y, x);
    }
    
    // Меняем параметры местами, чтобы было привычнее работать.
    uint32_t getPixColor_(int x, int y) {
        if (x > WIDTH - 1 || y > HEIGHT - 1)
            return 0;
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
    
    // Была ли нажата кнопка
    bool checkButtons(){
        if (buttons != 4) return true;
        return false;
    }

    // Если нужно, можно вызвать этот метод. Выводит отладочную информацию в терминал
    void printAll(){
        Serial.print("vectorX = ");
        Serial.print(vectorX);
        Serial.print(" vectorY = ");
        Serial.println(vectorY);

        Serial.print("headX = ");
        Serial.print(headX);
        Serial.print(" headY = ");
        Serial.println(headY);

        Serial.print("buttX = ");
        Serial.print(buttX);
        Serial.print(" buttY = ");
        Serial.println(buttY);

        Serial.print("appleX = ");
        Serial.print(appleX);
        Serial.print(" appleY = ");
        Serial.println(appleY);

        Serial.println("");
        Serial.print("buttVector = ");

        for (int i = 0; i < MAX_LENGTH; i++){
             Serial.print(buttVector[i]);
             Serial.print(" ");
        }

        Serial.println("---");
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
        genApple();

        // Тут мозг змеки
        if (gameDemo) snakeDemo_2();

        // Определяем куда идти змейке по нажатой кнопке
        buttonsTickSnake();

        // запоминаем, куда повернули голову
        // 1 - право, 3 - лево, 0 - вверх, 2 - вниз
        if (vectorX > 0) buttVector[snakeLength] = 1;
        else if (vectorX < 0) buttVector[snakeLength] = 3;
        if (vectorY > 0) buttVector[snakeLength] = 2;
        else if (vectorY < 0) buttVector[snakeLength] = 0;
        
        // смещение головы змеи
        headX += vectorX;
        headY += vectorY;

        if (headX < 0 || headX > WIDTH - 1 || headY < 0 || headY > HEIGHT - 1) { // если вышла за границы поля
            pizdetc = true;
        }

        if (snakeLength >= MAX_LENGTH) { // если максимальная длина змейки
            pizdetc = true;
        }

        if ((getPixColor_(headX, headY) != 0 && getPixColor_(headX, headY) != COLOR_APPLE)) {   // если змея врезалась во что то, но не в яблоко
            pizdetc = true;
        } 

        if (!pizdetc) {
            if (getPixColor_(headX, headY) == COLOR_APPLE) { // если попали головой в яблоко
                apple_flag = false;                       // флаг что яблока больше нет
                snakeLength++;                            // увеличить длину змеи
                missDelete = true;
                genApple();
            }

            // если змея не в процессе роста, закрасить бывший хвост чёрным
            if (!missDelete) {
                switch (buttVector[0]) {
                    case 0: buttY -= 1;
                        break;
                    case 1: buttX += 1;
                        break;
                    case 2: buttY += 1;
                        break;
                    case 3: buttX -= 1;
                        break;
                }

                for (byte i = 0; i < snakeLength; i++) {
                    buttVector[i] = buttVector[i + 1];
                }

                getPix_(buttX, buttY) = 0x000000;
            }
            else 
                missDelete = false;
        
            // рисуем голову змеи в новом положении
            getPix_(headX, headY) = COLOR_SNAKE;
        }

        if (pizdetc) {
            // ну в общем плавно моргнуть, типо змейке "больно"
            for (byte bright = 0; bright < 15; bright++) {
                FastLED.setBrightness(bright);
                for (int i = 0; i < LEDS_CNT; i++) {
                    leds[i] = CRGB::Red;
                }

                FastLED.show();
                delay(50);
            }

            //delay(100);
            //FastLED.clear();
            //FastLED.show();
            //FastLED.setBrightness(BRIGHTNESS);
            //displayScore(snakeLength - START_LENGTH);
            delay(100);
            FastLED.clear();
            FastLED.show();
            newGameSnake();      // миша, всё ху.я, давай по новой
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
                vectorY = -1;
                vectorX = 0;
                buttons = 4;
            }
            if (buttons == 2) {   // кнопка нажата
                vectorY = 1;
                vectorX = 0;
                buttons = 4;
            }
        }
    }
    
    void snakeDemo_2(){
        
        int8_t can_btn[4];
        // Ищем, в какую сторону можно двигаться. 
        // Проверяем на край поля и на наличие змейки на соседнем поле
        if (headY - 1 >= 0  && getPixColor_(headX, headY - 1) != COLOR_SNAKE)
            can_btn[0] = 1;
        else 
            can_btn[0] = -1;

        if (headX + 1 < WIDTH && getPixColor_(headX + 1, headY) != COLOR_SNAKE)
            can_btn[1] = 1;
        else 
            can_btn[1] = -1;
            
        if (headY + 1 < HEIGHT && getPixColor_(headX, headY + 1) != COLOR_SNAKE)
            can_btn[2] = 1;
        else 
            can_btn[2] = -1;

        if (headX - 1 >= 0 && getPixColor_(headX - 1, headY) != COLOR_SNAKE)
            can_btn[3] = 1;
        else 
            can_btn[3] = -1;
        
        // инициализируем кнопку. Сначала ничего не знаем, поэтому -1
        int8 b = -1;
        // Пытаемся понять в какой стороне яблоко
        int8 dX = headX - appleX;
        int8 dY = headY - appleY;
        
         if (dX == 0) {                // яблоко на одной линии по вертикали
            if (headY < appleY) b = 2;
            if (headY > appleY) b = 0;
        }

        if (dY == 0) {                // яблоко на одной линии по горизонтали
            if (headX < appleX) b = 1;
            if (headX > appleX) b = 3;
        }

        // Если не поняли куда идти или туда идти нельзя, то нужно придумать другой ход.
        if (b < 0 || can_btn[b] == -1){

            // Если пытались пойти против движения то сбрасываем направление и ищем дургое
            b = -1;

            // Пытаемся продолжить движение в том же направлении
            // 1 - право, 3 - лево, 0 - вверх, 2 - вниз
            if (vectorX > 0 && can_btn[1] != -1) b = 1;
            else if (vectorX < 0 && can_btn[3] != -1) b = 3;
            if (vectorY > 0 && can_btn[2] != -1) b = 2;
            else if (vectorY < 0 && can_btn[0] != -1) b = 0;

            // Если нету направления, то берем первое доступное
            uint8_t i = 0;
            while(b == -1 && i < 4){
                if (can_btn[i] > 0){
                    b = i;
                }
                i++;
            }
        }
        
        if (b > -1 && b < 4)
            buttons = b;
    }

    void snakeDemo() {
        // смещение головы змеи
        int8 nextX = headX + vectorX;
        int8 nextY = headY + vectorY;

        // ищем яблоко
        if (headX == appleX) {                // яблоко на одной линии по вертикали
            if (headY < appleY) buttons = 2;
            if (headY > appleY) buttons = 0;
        }
        if (headY == appleY) {                // яблоко на одной линии по горизонтали
            if (headX < appleX) buttons = 1;
            if (headX > appleX) buttons = 3;
        }

        if (getPixColor_(nextX, nextY) == COLOR_SNAKE) {   // проверка на столкновение с собой
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

    // Генерация яблока, если его нету на поле
    void genApple(){
        while (!apple_flag) {                         // пока яблоко не создано
            appleX = random(0, WIDTH);                  // взять случайные координаты
            appleY = random(0, HEIGHT);

            // проверить, не совпадает ли координата с телом змеи
            if ((long)getPixColor_(appleX, appleY) == 0) {
                apple_flag = true;                        // если не совпадает, считаем что яблоко создано
                getPix_(appleX, appleY) = COLOR_APPLE;        // и рисуем
                
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
            buttVector[i] = 1;
        }

        buttX = headX - snakeLength;   // координата хвоста как голова - длина

        missDelete = false;
        apple_flag = false;
        pizdetc = false;

        genApple();
    }

};