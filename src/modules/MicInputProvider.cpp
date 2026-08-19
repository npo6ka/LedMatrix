#include "MicInputProvider.h"

#if MIC_ENABLE && defined(ESP32DEV)

void MicInputProvider::fill(InputSnapshot &snapshot) const {
#if MIC_SPECTRUM_ENABLE
    const SpectrumAnalyzer *spectrum = _mic.spectrum();
    if (!spectrum || !_mic.isRunning()) {
        return;
    }

    snapshot.audio.available = true;
    snapshot.audio.bands = spectrum->bands();
    snapshot.audio.bandCount = spectrum->bandCount();
    snapshot.audio.rms = static_cast<float>(_mic.lastRms());
    snapshot.audio.dominantBand = spectrum->dominantBand();
#else
    (void)snapshot;
#endif
}

#endif
