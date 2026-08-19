#pragma once

#include "configs/constants.h"

#if MIC_ENABLE && defined(ESP32DEV)

#include "core/input/IInputProvider.h"
#include "modules/inmp441.h"

class MicInputProvider : public IInputProvider {
public:
    MicInputProvider() = default;

    InputCapability capability() const override { return InputCapability::Audio; }

    bool isAvailable() const override {
#if MIC_SPECTRUM_ENABLE
        return true;
#else
        return false;
#endif
    }

    void onActivate() override { _mic.start(); }
    void onDeactivate() override { _mic.stop(); }

    void tick() override { _mic.readAndAnalyze(); }

    void fill(InputSnapshot &snapshot) const override;

private:
    Inmp441 _mic;
};

#endif
