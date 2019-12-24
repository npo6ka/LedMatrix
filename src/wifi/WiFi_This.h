#pragma once

#include "ESP8266WiFi.h"
#include "ESP8266WebServer.h"

#define SSID "Android_IA"    // для внешней точки доступа
#define PASS "qwerty123"     // для внешней точки доступа

#define AP_SSID "Test_WiFi_NodeMCU" // для wifi на плате
#define AP_PASS "12345678"          // для wifi на плате

ESP8266WebServer server(80);

IPAddress apIP(192, 168, 4, 1);         //статический IP точки доступа

unsigned long tick_size = 1000;         // Частота срабатывания метода onTick(); в мс
unsigned long prev_micros_wifi_tick = 0;// последнее время срабатывания метода onTickWiFi()

void handleNotFound(){
    String message = "File Not Found \n\n";
    message += "URI: ";
    message += server.uri();
    message += "\nMethod: ";
    message += (server.method() == HTTP_GET)? "GET" : "POST";
    message += "\nArguments: ";
    message += server.args();
    message += "\n";
    for(uint8_t i = 0; i < server.args(); i++){
        message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
    }
    server.send(404, "text/plain", message);
}

void handleRoot(){
    Serial.print("handleRoot()");

    server.send(200, "text/plain", "Hello from esp8266!");
}

void handleRestart(){
    Serial.print("handleRestart()");

    String restart = server.arg("device");
    if (restart == "ok") ESP.restart();
    server.send(200, "text/plain", "OK");
}

void server_init(){
    server.onNotFound(handleNotFound);
    server.on("/", handleRoot);
    server.on("/restart", handleRestart);

    server.begin();
}

// Стартуем плату как точку доступа
void startOwnWiFi() {
    Serial.println("startOwnWiFi()");
    WiFi.disconnect();
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(apIP, apIP,  IPAddress(255, 255, 255, 0));
    WiFi.softAP(AP_SSID, AP_PASS);

    IPAddress myIP = WiFi.softAPIP();
    Serial.print("Access point Mode");
    Serial.print("AP IP address: ");
    Serial.println(myIP);
}

// Попытаться подключиться к роутеру
bool tryConnectToRouter(){
    WiFi.mode(WIFI_STA);
    byte tries = 6; // попыток подключения
    WiFi.begin(SSID, PASS);

    while (--tries && WiFi.status() != WL_CONNECTED){
        Serial.print(".");
        delay(1000);
    }
    return WiFi.status() == WL_CONNECTED;
}

void setupWiFi() { 
    if (!tryConnectToRouter()){
        Serial.print(" WiFi not found: ");
        Serial.print(SSID);
        Serial.println(". Starting your wifi...");
        startOwnWiFi();
        server_init();
    }else{
        Serial.println(" WiFi is successfully connected");
        IPAddress myIP = WiFi.localIP();
        Serial.print("Access point Mode");
        Serial.print("AP IP address: ");
        Serial.println(myIP);
    }
}

void onTickWiFi(){

    if (millis() - prev_micros_wifi_tick > tick_size){
        prev_micros_wifi_tick = millis();
        Serial.println("WiFiRouter::onTick()");

        server.handleClient();
        delay(1);
    }
}

