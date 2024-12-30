#pragma once

#include "ESP8266WiFi.h"
#include "ESP8266WebServer.h"

#define SSID "Keenetic-108_2.4"     // для внешней точки доступа
#define PASS "lost4815162342"       // для внешней точки доступа
#define NAME_ESP "Esp_1"            // Имя esp для отображении в мобильном приложении

class Wifi2024 {
private:
    ESP8266WebServer server; // Веб-сервер
public:
    Wifi2024();
    void onInit();
    void onTick();
    void http_root();
    void http_not_found();
    void http_esp();
};