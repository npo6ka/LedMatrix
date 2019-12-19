#include "WiFiRouter.h"


WiFiRouter& WiFiRouter::getInstance() {
    Serial.println("WiFiRouter::getInstance()");

    static WiFiRouter instance;
    return instance;
}

WiFiRouter::WiFiRouter() {
    init();
}

void WiFiRouter::init() {
    Serial.println("WiFiRouter::init()");

    WiFi.softAPConfig(IPAddress(IP_AP[0], IP_AP[1], IP_AP[2], IP_AP[3]),
                      IPAddress(192, 168, 4, 1),
                      IPAddress(255, 255, 255, 0));

    WiFi.softAP(AP_SSID, AP_PASS);
    IPAddress myIP = WiFi.softAPIP();
    Serial.print("Access point Mode");
    Serial.print("AP IP address: ");
    Serial.println(myIP);
}

void WiFiRouter::onTick() {
    if (micros() - prev_micros > tick_size){
        Serial.println("WiFiRouter::onTick()");
    }
}