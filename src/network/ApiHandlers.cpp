#include "ApiHandlers.h"

#include "core/effect/EffectFactory/EffectFactory.h"
#include "core/effect/EffectInfo.h"
#include "network/DeferredActions.h"

#include <ArduinoJson.h>

namespace {
    portMUX_TYPE rateLimitMux = portMUX_INITIALIZER_UNLOCKED;

    void sendJson(AsyncWebServerRequest* request, int code, const JsonDocument& doc) {
        String payload;
        payload.reserve(measureJson(doc) + 1);
        const size_t len = serializeJson(doc, payload);
        if (len == 0) {
            request->send(500, "application/json", "{\"ok\":false,\"error\":\"json serialize failed\"}");
            return;
        }
        request->send(code, "application/json", payload);
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

    bool postAction(const DeferredAction& action, AsyncWebServerRequest* request) {
        if (!DeferredActions::post(action)) {
            sendError(request, 503, "busy");
            return false;
        }
        sendOk(request);
        return true;
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
        doc["symmetric"] = snap.symmetric;
        doc["width"] = snap.width;
        doc["height"] = snap.height;
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

            DeferredAction action{};
            action.type = DeferredActionType::PowerSet;
            action.data.boolValue = doc["on"].as<bool>();
            postAction(action, request);
        });

    server.on("/api/mode/next", HTTP_POST, [this](AsyncWebServerRequest* request) {
        if (!checkRateLimit()) {
            sendError(request, 429, "rate limit");
            return;
        }
        DeferredAction action{};
        action.type = DeferredActionType::ModeNext;
        postAction(action, request);
    });

    server.on("/api/mode/prev", HTTP_POST, [this](AsyncWebServerRequest* request) {
        if (!checkRateLimit()) {
            sendError(request, 429, "rate limit");
            return;
        }
        DeferredAction action{};
        action.type = DeferredActionType::ModePrev;
        postAction(action, request);
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
            DeferredAction action{};
            action.type = DeferredActionType::ModeSet;
            action.data.indexValue = index;
            postAction(action, request);
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

            DeferredAction action{};
            action.type = DeferredActionType::AutoModSet;
            action.data.boolValue = doc["enabled"].as<bool>();
            postAction(action, request);
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

            DeferredAction action{};
            action.type = DeferredActionType::BrightnessSet;
            action.data.intValue = value;
            postAction(action, request);
        });

    server.on("/api/symmetric", HTTP_POST, [](AsyncWebServerRequest* request) {}, nullptr,
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

            DeferredAction action{};
            action.type = DeferredActionType::SymmetricSet;
            action.data.boolValue = doc["enabled"].as<bool>();
            postAction(action, request);
        });

    server.on("/api/reset", HTTP_POST, [this](AsyncWebServerRequest* request) {
        if (!checkRateLimit()) {
            sendError(request, 429, "rate limit");
            return;
        }
        DeferredAction action{};
        action.type = DeferredActionType::ResetModesList;
        postAction(action, request);
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
