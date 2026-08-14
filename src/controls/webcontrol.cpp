#include "webcontrol.h"

#if WIFI_ENABLE && defined(ESP32DEV)

#include "controls/automode.h"
#include "libs/led_matrix.h"

void WebControl::onInit(
    IEffectStorage* storage,
    EffectManager* effectManager,
    bool* powerOn,
    AutoChangeMode* autoMod) {
    _powerOn = powerOn;
    _autoMod = autoMod;

    _wifiAp.begin();

    _status.bind(
        storage,
        effectManager,
        [this]() { return _powerOn && *_powerOn; },
        [this]() { return _autoMod && _autoMod->isEnable(); },
        []() { return LedMatrix.getBrightness(); });
    _status.setNetworkInfo(_wifiAp.getIp(), _wifiAp.getSsid());

    _webServer.begin(_status);
}

void WebControl::onTick() {
    _webServer.onTick();
}

#endif
