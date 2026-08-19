#pragma once

#include "configs/constants.h"

#if MIC_ENABLE && defined(ESP32DEV)

#include "libs/StdFeatures.h"
#include "modules/spectrum_analyzer.h"

class Inmp441 {
public:
    Inmp441();

    void onInit();
    void onTick();

    const SpectrumAnalyzer *spectrum() const { return _spectrum.get(); }

private:
    bool _ok = false;
#if MIC_SPECTRUM_ENABLE
    std::unique_ptr<SpectrumAnalyzer> _spectrum;
    bool _useLeftChannel = true;
#endif
};

#endif
