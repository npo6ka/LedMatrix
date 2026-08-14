#include "WiFiAp.h"

#include "configs/matrix.h"
#include "libs/debug_lib.h"

#include <WiFi.h>
#include <esp_mac.h>

bool WiFiAp::begin() {
    WiFi.persistent(false);
    WiFi.mode(WIFI_AP);

    uint8_t mac[6];
    esp_read_mac(mac, ESP_MAC_WIFI_SOFTAP);
    char ssidSuffix[5];
    snprintf(ssidSuffix, sizeof(ssidSuffix), "%02X%02X", mac[4], mac[5]);
    _ssid = String(WIFI_AP_SSID) + "-" + ssidSuffix;

    const char* password = WIFI_AP_PASSWORD;
    if (password[0] == '\0') {
        WiFi.softAP(_ssid.c_str(), nullptr, WIFI_AP_CHANNEL, false, WIFI_AP_MAX_CLIENTS);
    } else {
        WiFi.softAP(_ssid.c_str(), password, WIFI_AP_CHANNEL, false, WIFI_AP_MAX_CLIENTS);
    }

    // Дать драйверу WiFi завершить поднятие AP перед mDNS/HTTP
    delay(100);

    _ip = WiFi.softAPIP().toString();
    logInfo("WiFi AP: SSID=%s IP=%s\n", _ssid.c_str(), _ip.c_str());
    if (password[0] != '\0') {
        logInfo("WiFi AP password: %s\n", password);
    }
    return true;
}

String WiFiAp::getIp() const {
    return _ip;
}

String WiFiAp::getSsid() const {
    return _ssid;
}
