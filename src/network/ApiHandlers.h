#pragma once

#include "AppStatus.h"

#include <ESPAsyncWebServer.h>

class ApiHandlers {
public:
    void registerRoutes(AsyncWebServer& server, AppStatus& status);

private:
    bool checkRateLimit();
    void registerCaptivePortal(AsyncWebServer& server);

    unsigned long _lastPostMs = 0;
    AppStatus* _status = nullptr;
};
