#pragma once

#include <cstdint>

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>

enum class DeferredActionType : uint8_t {
    ModeNext,
    ModePrev,
    ModeSet,
    PowerSet,
    AutoModSet,
    BrightnessSet,
    ResetModesList,
};

struct DeferredAction {
    DeferredActionType type;
    union {
        bool boolValue;
        int intValue;
        uint16_t indexValue;
    } data;
};

class DeferredActions {
public:
    static bool init();
    static bool post(const DeferredAction& action);
    static void process();

private:
    static QueueHandle_t _queue;
};
