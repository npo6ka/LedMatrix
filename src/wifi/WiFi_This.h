#pragma once

#include "ESP8266WiFi.h"
#include "ESP8266WebServer.h"
#include "effects/_19_Empty.h"

#define IsTryToConnectRouter false    // Попытаться подключиться к внешнему роутеру

#define SSID "Android_IA"    // для внешней точки доступа
#define PASS "qwerty123"     // для внешней точки доступа

#define AP_SSID "Test_WiFi_NodeMCU" // для wifi на плате
#define AP_PASS "12345678"          // для wifi на плате

ESP8266WebServer server(80);

IPAddress apIP(192, 168, 4, 1);         //статический IP точки доступа

unsigned long tick_size = 10;         // Частота срабатывания метода onTick(); в мс
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

// Метод срабатывает, когда получен запрос http:192.168.4.1/command
// Нету проверки на пустые значения!!!!
void handleCommand(){
    Serial.println("handleCommand()");
    server.send(200, "text/plain", "OK");
    
    String com = server.arg("c");
    String param = server.arg("p");
    
    Serial.print("Command = ");
    Serial.println(com);
    Serial.print("Parametr = ");
    Serial.println(param);
}

// Метод срабатывает, когда получен запрос http:192.168.4.1/next
void handleNext(){
    Serial.println("handleNext()");
    server.send(200, "text/plain", "OK");
    
    EffectsList::getInstance().nextEffect();
}

// Метод срабатывает, когда получен запрос http:192.168.4.1/prev
void handlePrev(){
    Serial.println("handlePrev()");
    server.send(200, "text/plain", "OK");
    
    EffectsList::getInstance().prevEffect();
}

// Метод срабатывает, когда получен запрос http:192.168.4.1/prev
void handleSetMode(){
    Serial.println("handleSetMode()");
    server.send(200, "text/plain", "OK");

    bool isAutomod = server.arg("isAutoMod");
    int num = atoi(server.arg("num").c_str());


    Serial.print("isAutomod");
    Serial.println(isAutomod);

    if (num == 100){
        EffectsList::getInstance().setNadysText();
    }else{
        if (num == 200){
            EffectsList::getInstance().setEmptyMode();
        }
    }

}

// Метод срабатывает, когда получен запрос http:192.168.4.1/switchled
void handleswitchLed(){
    Serial.println("handleNext()");
    server.send(200, "text/plain", "OK");
    
    int w = atoi(server.arg("w").c_str());
    int h = atoi(server.arg("h").c_str());
    int c = atoi(server.arg("c").c_str());
    
    // Serial.print("w = ");
    // Serial.println(w);
    // Serial.print("h = ");
    // Serial.println(h);
    // Serial.print("c = ");
    // Serial.println(c);

    getPix(h, w) = c;
    //EffectsList::getInstance().prevEffect()
    //Effect *e = EffectsList::getInstance().getCurEffect();
    //((Empty)e)
}

void server_init(){
    server.onNotFound(handleNotFound);
    server.on("/command", handleCommand);
    server.on("/next", handleNext);
    server.on("/switchled", handleswitchLed);
    server.on("/prev", handlePrev);
    server.on("/setMode", handleSetMode);
    server.begin();
}

// Стартуем плату как точку доступа
void startOwnWiFi() {
    Serial.println("startOwnWiFi()");
    WiFi.disconnect();
    WiFi.mode(WIFI_AP);
    WiFi.softAPConfig(apIP, apIP,  IPAddress(255, 255, 255, 0));
    WiFi.softAP(AP_SSID, AP_PASS);
}

// Попытаться подключиться к роутеру
bool tryConnectToRouter(){
    if (!IsTryToConnectRouter)
        return false;

    byte tries = 6; // попыток подключения

    WiFi.mode(WIFI_STA);
    WiFi.begin(SSID, PASS);

    while (--tries && WiFi.status() != WL_CONNECTED){
        Serial.print(".");
        delay(1000);
    }
    return WiFi.status() == WL_CONNECTED;
}

// Вызвать этот метод в main setup()
void setupWiFi() { 
    if (!tryConnectToRouter()){
        Serial.print(" WiFi not found: ");
        Serial.print(SSID);
        Serial.println(". Starting your wifi...");
        startOwnWiFi();
    }else{
        Serial.println(" WiFi is successfully connected");
        IPAddress myIP = WiFi.localIP();
        Serial.print("Access point Mode");
        Serial.print("AP IP address: ");
        Serial.println(myIP);
    }
    
    server_init();
}

// Вызвать этот метод в main loop()
void onTickWiFi(){

    if (millis() - prev_micros_wifi_tick > tick_size){
        prev_micros_wifi_tick = millis();
        //Serial.println("WiFiRouter::onTick()");

        server.handleClient();
        delay(1);
    }
}

