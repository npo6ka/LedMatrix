#pragma once

// #include "NTPClient.h"
#include "ESP8266WiFi.h"
#include "ESP8266WebServer.h"

#define SSID "Keenetic-108_2.4"     // для внешней точки доступа
#define PASS "lost4815162342"       // для внешней точки доступа

// #define AP_SSID "Test_WiFi_NodeMCU" // для wifi на плате
// #define AP_PASS "12345678"          // для wifi на плате

unsigned long tick_size = 1000;         // Частота срабатывания метода onTick(); в мс
unsigned long prev_micros_wifi_tick = 0;// последнее время срабатывания метода onTickWiFi()

// Вызвать этот метод в main setup()
void setupWiFi() { 
    // if (!tryConnectToRouter()){
    //     Serial.print(" WiFi not found: ");
    //     Serial.print(SSID);
    //     Serial.println(". Starting your wifi...");
    //     startOwnWiFi();
    // }else{
    //     Serial.println(" WiFi is successfully connected");
    //     IPAddress myIP = WiFi.localIP();
    //     Serial.print("Access point Mode");
    //     Serial.print("AP IP address: ");
    //     Serial.println(myIP);
    // }
    
    // server_init();
}

bool try_connect(){

    return false;
}

// Вызвать этот метод в main loop()
void tick_wifi(){
    if (millis() - prev_micros_wifi_tick > tick_size){
        prev_micros_wifi_tick = millis();
        out("wifi_timer::tick_wifi()   millis = %d\n", prev_micros_wifi_tick);
    }
}

