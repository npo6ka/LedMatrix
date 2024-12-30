#include "wifi2024.h"

Wifi2024::Wifi2024(): server(8081){
    
}

void Wifi2024::onInit() {
    pinMode(LED_BUILTIN, OUTPUT);

    Serial.println();
    Serial.print("Configuring WIFI access point... "); 
    Serial.println(SSID);
    
    WiFi.begin(SSID, PASS);
    while (WiFi.status() != WL_CONNECTED) {
        // Мигаем диодом, чтобы видеть попытки подключиться к wifi
        digitalWrite(LED_BUILTIN, HIGH);
        delay(250);
        digitalWrite(LED_BUILTIN, LOW);
        delay(250);
        Serial.print(".");
    }
    digitalWrite(LED_BUILTIN, HIGH);

    Serial.print("IP-адрес: ");
    Serial.println(WiFi.localIP());

    server.on("/", [this]() { http_root(); });
    server.onNotFound([this]() { http_not_found(); }); // Для всех остальных запросов
    server.on("/esp/", [this]() { http_esp(); }); // Отправляем тут свой ключ и своё имя

    server.begin();
    Serial.println("HTTP server started!");
}

void Wifi2024::onTick() {
    server.handleClient();
}

void Wifi2024::http_root() {
    server.send(200, "text/html", "<h1>Base HTML page this.</h1>");
}

void Wifi2024::http_not_found() {
    server.send(404, "text/html", "<h1>Not Found!</h1>");
}

void Wifi2024::http_esp() {
    String code = server.arg("code");
    Serial.print("Code: ");
    Serial.println(code);

    server.send(200, "application/json", "{'code': 'ead2c4f3-ed9b-4345-8ba8-6f446d13fc5d', 'name': 'Esp_1'}");
}
