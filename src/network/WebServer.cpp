#include "WebServer.h"

#include "configs/matrix.h"
#include "libs/debug_lib.h"

#include <ESPmDNS.h>

namespace {

void registerStaticFiles(AsyncWebServer& server) {
    server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send(LittleFS, "/index.html", "text/html");
    });
    server.on("/index.html", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send(LittleFS, "/index.html", "text/html");
    });
    server.on("/app.js", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send(LittleFS, "/app.js", "application/javascript");
    });
    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send(LittleFS, "/style.css", "text/css");
    });
}

} // namespace

bool MatrixWebServer::begin(AppStatus& status) {
    _status = &status;

    if (!LittleFS.begin(false)) {
        if (!LittleFS.begin(true)) {
            logError("LittleFS mount failed\n");
            return false;
        }
    }

    _api.registerRoutes(_server, status);
    registerStaticFiles(_server);
    _server.begin();
#if defined(WIFI_MDNS_HOST)
    if (MDNS.begin(WIFI_MDNS_HOST)) {
        MDNS.addService("http", "tcp", 80);
        logInfo("mDNS: http://%s.local\n", WIFI_MDNS_HOST);
    } else {
        logError("mDNS start failed\n");
    }
#endif

    logInfo("HTTP server started on port 80\n");
    return true;
}

void MatrixWebServer::onTick() {
    if (_status) {
        _status->updateFps();
    }
}
