#include "ApiHandlers.h"

#include "core/effect/EffectFactory/EffectFactory.h"
#include "core/effect/EffectInfo.h"
#include "events/ChangeModeEventRequest.h"
#include "events/observer.h"

#include <ArduinoJson.h>

namespace {
    portMUX_TYPE rateLimitMux = portMUX_INITIALIZER_UNLOCKED;

    void sendJson(AsyncWebServerRequest* request, int code, const JsonDocument& doc) {
        String body;
        serializeJson(doc, body);
        request->send(code, "application/json", body);
    }

    void sendOk(AsyncWebServerRequest* request) {
        request->send(200, "application/json", "{\"ok\":true}");
    }

    void sendError(AsyncWebServerRequest* request, int code, const char* message) {
        JsonDocument doc;
        doc["ok"] = false;
        doc["error"] = message;
        sendJson(request, code, doc);
    }
} // namespace

bool ApiHandlers::checkRateLimit() {
    const unsigned long now = millis();
    portENTER_CRITICAL(&rateLimitMux);
    const bool allowed = (now - _lastPostMs >= 200UL);
    if (allowed) {
        _lastPostMs = now;
    }
    portEXIT_CRITICAL(&rateLimitMux);
    return allowed;
}

void ApiHandlers::registerCaptivePortal(AsyncWebServer& server) {
    auto redirectRoot = [](AsyncWebServerRequest* request) {
        request->redirect("/");
    };

    server.on("/generate_204", HTTP_GET, redirectRoot);
    server.on("/hotspot-detect.html", HTTP_GET, redirectRoot);
    server.on("/connecttest.txt", HTTP_GET, redirectRoot);
    server.on("/fwlink", HTTP_GET, redirectRoot);
}

void ApiHandlers::registerRoutes(AsyncWebServer& server, AppStatus& status) {
    _status = &status;

    server.on("/api/status", HTTP_GET, [this](AsyncWebServerRequest* request) {
        const AppStatusSnapshot snap = _status->snapshot();
        JsonDocument doc;
        doc["power"] = snap.powerOn;
        doc["autoMode"] = snap.autoMode;
        doc["effectIndex"] = snap.effectIndex;
        doc["effectId"] = snap.effectId;
        doc["effectName"] = snap.effectName;
        doc["fps"] = snap.fps;
        doc["brightness"] = snap.brightness;
        doc["effectsCount"] = snap.effectsCount;
        doc["ip"] = snap.ip;
        doc["ssid"] = snap.ssid;
        sendJson(request, 200, doc);
    });

    server.on("/api/effects", HTTP_GET, [this](AsyncWebServerRequest* request) {
        JsonDocument doc;
        JsonArray effects = doc.to<JsonArray>();

        IEffectStorage* storage = _status->storage();
        if (!storage) {
            sendError(request, 500, "storage unavailable");
            return;
        }

        for (size_t i = 0; i < storage->size(); ++i) {
            const EffectInfo& info = storage->getEffectInfo(i);
            JsonObject item = effects.add<JsonObject>();
            item["index"] = i;
            item["id"] = info.id;
            item["name"] = EffectFactory::getEffectName(info.id);
        }

        sendJson(request, 200, doc);
    });

    server.on("/api/power", HTTP_POST, [](AsyncWebServerRequest* request) {}, nullptr,
        [this](AsyncWebServerRequest* request, uint8_t* data, size_t len, size_t, size_t) {
            if (!checkRateLimit()) {
                sendError(request, 429, "rate limit");
                return;
            }

            JsonDocument doc;
            if (deserializeJson(doc, data, len)) {
                sendError(request, 400, "invalid json");
                return;
            }

            if (!doc["on"].is<bool>()) {
                sendError(request, 400, "missing on");
                return;
            }

            Observable::notify<ChangeBoolEvent>(EventType::SetPowerState, doc["on"].as<bool>());
            sendOk(request);
        });

    server.on("/api/mode/next", HTTP_POST, [this](AsyncWebServerRequest* request) {
        if (!checkRateLimit()) {
            sendError(request, 429, "rate limit");
            return;
        }
        Observable::notify<ChangeModeEvent>(
            EventType::ChangeMode, true, ChangeModeEventRequest::Type::Next);
        sendOk(request);
    });

    server.on("/api/mode/prev", HTTP_POST, [this](AsyncWebServerRequest* request) {
        if (!checkRateLimit()) {
            sendError(request, 429, "rate limit");
            return;
        }
        Observable::notify<ChangeModeEvent>(
            EventType::ChangeMode, true, ChangeModeEventRequest::Type::Previous);
        sendOk(request);
    });

    server.on("/api/mode", HTTP_POST, [](AsyncWebServerRequest* request) {}, nullptr,
        [this](AsyncWebServerRequest* request, uint8_t* data, size_t len, size_t, size_t) {
            if (!checkRateLimit()) {
                sendError(request, 429, "rate limit");
                return;
            }

            JsonDocument doc;
            if (deserializeJson(doc, data, len)) {
                sendError(request, 400, "invalid json");
                return;
            }

            if (!doc["index"].is<uint16_t>() && !doc["index"].is<int>()) {
                sendError(request, 400, "missing index");
                return;
            }

            const uint16_t index = doc["index"].as<uint16_t>();
            Observable::notify<ChangeModeEvent>(
                EventType::ChangeMode, true, ChangeModeEventRequest::Type::Set, index);
            sendOk(request);
        });

    server.on("/api/automode", HTTP_POST, [](AsyncWebServerRequest* request) {}, nullptr,
        [this](AsyncWebServerRequest* request, uint8_t* data, size_t len, size_t, size_t) {
            if (!checkRateLimit()) {
                sendError(request, 429, "rate limit");
                return;
            }

            JsonDocument doc;
            if (deserializeJson(doc, data, len)) {
                sendError(request, 400, "invalid json");
                return;
            }

            if (!doc["enabled"].is<bool>()) {
                sendError(request, 400, "missing enabled");
                return;
            }

            Observable::notify<ChangeBoolEvent>(EventType::SetAutoMod, doc["enabled"].as<bool>());
            sendOk(request);
        });

    server.on("/api/brightness", HTTP_POST, [](AsyncWebServerRequest* request) {}, nullptr,
        [this](AsyncWebServerRequest* request, uint8_t* data, size_t len, size_t, size_t) {
            if (!checkRateLimit()) {
                sendError(request, 429, "rate limit");
                return;
            }

            JsonDocument doc;
            if (deserializeJson(doc, data, len)) {
                sendError(request, 400, "invalid json");
                return;
            }

            if (!doc["value"].is<int>()) {
                sendError(request, 400, "missing value");
                return;
            }

            int value = doc["value"].as<int>();
            if (value < 0) value = 0;
            if (value > 255) value = 255;

            Observable::notify<ChangeIntEvent>(EventType::SetBrightness, value);
            sendOk(request);
        });

    server.on("/api/reset", HTTP_POST, [this](AsyncWebServerRequest* request) {
        if (!checkRateLimit()) {
            sendError(request, 429, "rate limit");
            return;
        }
        Observable::notify<Event>(EventType::ResetModesList);
        sendOk(request);
    });

    registerCaptivePortal(server);

    server.onNotFound([](AsyncWebServerRequest* request) {
        if (request->method() == HTTP_OPTIONS) {
            request->send(204);
            return;
        }
        if (request->url().startsWith("/api/")) {
            request->send(404, "application/json", "{\"ok\":false,\"error\":\"not found\"}");
            return;
        }
        request->send(404, "text/plain", "Not found");
    });
}
