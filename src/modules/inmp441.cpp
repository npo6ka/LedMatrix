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

void fillChannelStats(const int32_t *raw, int count, int stride, int32_t &dc, int32_t &rms) {
    dc = 0;
    rms = 0;
    if (count <= 0) {
        return;
    }

    int64_t sum = 0;
    int64_t acc = 0;
    for (int i = 0; i < count; ++i) {
        const int32_t s = toSample(raw[i * stride]);
        sum += s;
        acc += static_cast<int64_t>(s) * s;
    }

    dc = static_cast<int32_t>(sum / count);
    rms = static_cast<int32_t>(sqrt(static_cast<double>(acc) / count));
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

    int32_t leftDc = 0;
    int32_t leftRms = 0;
    int32_t rightDc = 0;
    int32_t rightRms = 0;
    fillChannelStats(frames, pairs, 2, leftDc, leftRms);
    fillChannelStats(frames + 1, pairs, 2, rightDc, rightRms);

    _useLeftChannel = leftRms >= rightRms;
    const int32_t activeDc = _useLeftChannel ? leftDc : rightDc;
    _lastRms = _useLeftChannel ? leftRms : rightRms;

#if MIC_SPECTRUM_ENABLE
    if (_spectrum) {
        int32_t mono[kFrames];
        extractMono(frames, pairs, _useLeftChannel, mono);
        if (_spectrum->pushSamples(mono, static_cast<size_t>(pairs), activeDc)) {
            _spectrum->clearReady();
        }
    }
#endif
}

#endif
