#pragma once

#include "ApiHandlers.h"
#include "AppStatus.h"

#include <ESPAsyncWebServer.h>
#include <LittleFS.h>

class MatrixWebServer {
public:
    bool begin(AppStatus& status);
    void onTick();

private:
    AsyncWebServer _server{80};
    ApiHandlers _api;
    AppStatus* _status = nullptr;
};
