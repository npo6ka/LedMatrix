#include "inmp441.h"

#if MIC_ENABLE && defined(ESP32DEV)

#include "libs/StdFeatures.h"
#include "libs/debug_lib.h"

#include <Arduino.h>
#include <driver/i2s.h>
#include <math.h>

namespace {

constexpr i2s_port_t kPort = I2S_NUM_0;
constexpr int kFrames = 128;

int32_t toSample(int32_t raw) {
    // INMP441: 24-bit sample in upper bits of 32-bit I2S slot
    return raw >> 14;
}

void fillChannelStats(const int32_t *raw, int count, int stride, int32_t &acRms) {
    acRms = 0;
    if (count <= 0) {
        return;
    }

    int64_t sum = 0;
    for (int i = 0; i < count; ++i) {
        const int32_t s = toSample(raw[i * stride]);
        sum += s;
    }

    const int32_t dc = static_cast<int32_t>(sum / count);

    int64_t centeredAcc = 0;
    for (int i = 0; i < count; ++i) {
        const int64_t centered = static_cast<int64_t>(toSample(raw[i * stride])) - dc;
        centeredAcc += centered * centered;
    }
    acRms = static_cast<int32_t>(sqrt(static_cast<double>(centeredAcc) / count));
}

void extractMono(const int32_t *frames, int pairs, bool useLeft, int32_t *mono) {
    const int offset = useLeft ? 0 : 1;
    for (int i = 0; i < pairs; ++i) {
        mono[i] = toSample(frames[i * 2 + offset]);
    }
}

} // namespace

Inmp441::Inmp441() {
#if MIC_SPECTRUM_ENABLE
    _spectrum = std::make_unique<SpectrumAnalyzer>(MIC_SAMPLE_RATE, MIC_FFT_SIZE, MIC_SPECTRUM_BANDS);
#endif
}

void Inmp441::start() {
    if (_running) {
        return;
    }

    i2s_config_t cfg = {};
    cfg.mode = static_cast<i2s_mode_t>(I2S_MODE_MASTER | I2S_MODE_RX);
    cfg.sample_rate = MIC_SAMPLE_RATE;
    cfg.bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT;
    cfg.channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT;
    cfg.communication_format = I2S_COMM_FORMAT_STAND_I2S;
    cfg.intr_alloc_flags = ESP_INTR_FLAG_LEVEL1;
    cfg.dma_buf_count = 4;
    cfg.dma_buf_len = kFrames;
    cfg.use_apll = false;
    cfg.tx_desc_auto_clear = false;
    cfg.fixed_mclk = 0;
    cfg.mclk_multiple = I2S_MCLK_MULTIPLE_256;
    cfg.bits_per_chan = I2S_BITS_PER_CHAN_32BIT;

    i2s_pin_config_t pins = {};
    pins.mck_io_num = I2S_PIN_NO_CHANGE;
    pins.bck_io_num = I2S_SCK;
    pins.ws_io_num = I2S_WS;
    pins.data_out_num = I2S_PIN_NO_CHANGE;
    pins.data_in_num = I2S_SD;

    esp_err_t err = i2s_driver_install(kPort, &cfg, 0, nullptr);
    if (err != ESP_OK) {
        logError("INMP441: i2s_driver_install failed: %d\n", static_cast<int>(err));
        return;
    }

    err = i2s_set_pin(kPort, &pins);
    if (err != ESP_OK) {
        logError("INMP441: i2s_set_pin failed: %d\n", static_cast<int>(err));
        i2s_driver_uninstall(kPort);
        return;
    }

    i2s_zero_dma_buffer(kPort);
    _running = true;
    _filterInitialized = false;
    _hpPrevInput = 0.0f;
    _hpPrevOutput = 0.0f;

#if MIC_SPECTRUM_ENABLE
    if (_spectrum) {
        _spectrum->reset();
    }
#endif
}

void Inmp441::stop() {
    if (!_running) {
        return;
    }

    i2s_driver_uninstall(kPort);
    _running = false;
    _lastRms = 0;
    _filterInitialized = false;
    _hpPrevInput = 0.0f;
    _hpPrevOutput = 0.0f;
}

void Inmp441::readAndAnalyze() {
    if (!_running) {
        return;
    }

    int32_t frames[kFrames * 2];
    size_t bytesRead = 0;
    const esp_err_t err = i2s_read(kPort, frames, sizeof(frames), &bytesRead, pdMS_TO_TICKS(40));
    const int pairs = static_cast<int>(bytesRead / (2 * sizeof(int32_t)));

    if (err != ESP_OK || pairs <= 0) {
        return;
    }

    int32_t leftAcRms = 0;
    int32_t rightAcRms = 0;
    fillChannelStats(frames, pairs, 2, leftAcRms);
    fillChannelStats(frames + 1, pairs, 2, rightAcRms);

    _useLeftChannel = leftAcRms >= rightAcRms;

#if MIC_SPECTRUM_ENABLE
    if (_spectrum) {
        int32_t mono[kFrames];
        extractMono(frames, pairs, _useLeftChannel, mono);
        constexpr float kHighPassAlpha = 0.9865f; // около 35 Гц при 16 кГц
        int64_t filteredSquares = 0;
        for (int i = 0; i < pairs; ++i) {
            const float input = static_cast<float>(mono[i]);
            if (!_filterInitialized) {
                _hpPrevInput = input;
                _hpPrevOutput = 0.0f;
                _filterInitialized = true;
            }
            const float output = kHighPassAlpha * (_hpPrevOutput + input - _hpPrevInput);
            _hpPrevInput = input;
            _hpPrevOutput = output;
            mono[i] = static_cast<int32_t>(output);
            filteredSquares += static_cast<int64_t>(mono[i]) * mono[i];
        }
        _lastRms = pairs > 0
            ? static_cast<int32_t>(sqrt(static_cast<double>(filteredSquares) / pairs))
            : 0;

        if (_spectrum->pushSamples(mono, static_cast<size_t>(pairs), 0)) {
            _spectrum->clearReady();
        }
    }
#endif
}

#endif
