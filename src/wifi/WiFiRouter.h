#pragma once

#include "ESP8266WiFi.h"
#include "ESP8266WebServer.h"
#include "stdint.h"

#define AP_SSID "Test_WiFi_NodeMCU"
#define AP_PASS "123456"
#define AP_PORT 8888

class WiFiRouter
{
private:
    // singlton property
    // Конструкторы и оператор присваивания недоступны клиентам
    WiFiRouter();
    WiFiRouter( const WiFiRouter& );  
    WiFiRouter& operator=( WiFiRouter& );
    
    uint8_t IP_AP[4] = {192, 168, 4, 66};   // статический IP точки доступа (менять только последнюю цифру) 
    unsigned long tick_size = 1000;         // Частота срабатывания метода onTick(); в мс

    unsigned long prev_micros = 0;
    void init();

public:
    static WiFiRouter& getInstance();

    void onTick();
};