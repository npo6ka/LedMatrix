#pragma once

#include <Arduino.h>

class WiFiAp {
public:
    bool begin();
    String getIp() const;
    String getSsid() const;

private:
    String _ssid;
    String _ip;
};
