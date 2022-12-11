#pragma once

#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#define SSID "Keenetic-108_2.4"     // для внешней точки доступа
#define PASS "lost4815162342"       // для внешней точки доступа

// #define AP_SSID "Test_WiFi_NodeMCU" // для wifi на плате
// #define AP_PASS "12345678"          // для wifi на плате

unsigned long tick_size = 5000;         // Частота срабатывания метода onTick(); в мс
unsigned long prev_micros_wifi_tick = 0;// последнее время срабатывания метода onTickWiFi()

unsigned long utcOffsetInSeconds = 3600;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

long time_this = 0;

// Определение NTP-клиента для получения времени
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

// Вызвать этот метод в main setup()
void setupWiFi() { 

    WiFi.begin(SSID, PASS);

    while ( WiFi.status() != WL_CONNECTED ){
        delay(1000);
        Serial.print(".");
    }

    timeClient.begin();
}

bool try_connect(){

    return false;
}

// Вызвать этот метод в main loop()
void tick_wifi(){
    if (millis() - prev_micros_wifi_tick > tick_size){
        prev_micros_wifi_tick = millis();
        
        if (time_this < 10000){
            Serial.println("time_this < 10000");
            time_this = timeClient.getEpochTime();
        }else{
            Serial.println("time_this >= 10000");
            time_this = time_this + tick_size;
        }

        timeClient.update();
        Serial.print(daysOfTheWeek[timeClient.getDay()]);
        Serial.print(", ");
        Serial.print(timeClient.getHours());
        Serial.print(":");
        Serial.print(timeClient.getMinutes());
        Serial.print(":");
        Serial.println(timeClient.getSeconds());
                
        Serial.print("delta = ");
        long r = timeClient.getEpochTime();
        
        Serial.println(r);
        r = r - time_this;
        Serial.println(r);
    }
}

