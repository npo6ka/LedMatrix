#pragma once

#include "configs/constants.h"

#if MIC_ENABLE && defined(ESP32DEV)

#include "libs/StdFeatures.h"
#include "modules/SpectrumAnalyzer.h"

class Inmp441 {
public:
    Inmp441();

    void start();
    void stop();
    bool isRunning() const { return _running; }

    void readAndAnalyze();

    const SpectrumAnalyzer *spectrum() const { return _spectrum.get(); }
    int32_t lastRms() const { return _lastRms; }

private:
    bool _running = false;
    bool _useLeftChannel = true;
    int32_t _lastRms = 0;
#if MIC_SPECTRUM_ENABLE
    std::unique_ptr<SpectrumAnalyzer> _spectrum;
#endif
};

#endif
