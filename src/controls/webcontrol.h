#pragma once

#include "configs/matrix.h"

#if WIFI_ENABLE && defined(ESP32DEV)

#include "core/effect/EffectManager.h"
#include "core/effect/storage/IEffectStorage.h"
#include "network/AppStatus.h"
#include "network/WiFiAp.h"
#include "network/WebServer.h"

class AutoChangeMode;

class WebControl {
public:
    void onInit(
        IEffectStorage* storage,
        EffectManager* effectManager,
        bool* powerOn,
        AutoChangeMode* autoMod);
    void onTick();

private:
    WiFiAp _wifiAp;
    AppStatus _status;
    MatrixWebServer _webServer;
    bool* _powerOn = nullptr;
    AutoChangeMode* _autoMod = nullptr;
};

#endif
