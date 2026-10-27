#include "microphone.h"
#include "board_pins.h"
#include <driver/i2s.h>

namespace Microphone {
bool begin() {
    i2s_config_t cfg = {};
    cfg.mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX);
    cfg.sample_rate = SAMPLE_RATE;
    cfg.bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT;
    cfg.channel_format = I2S_CHANNEL_FMT_ONLY_LEFT;
    cfg.communication_format = I2S_COMM_FORMAT_STAND_I2S;
    cfg.dma_buf_count = 8;
    cfg.dma_buf_len = 256;
    cfg.use_apll = false;
    i2s_pin_config_t pins = {};
    pins.mck_io_num = I2S_PIN_NO_CHANGE;
    pins.bck_io_num = Pins::MIC_BCLK;
    pins.ws_io_num = Pins::MIC_WS;
    pins.data_in_num = Pins::MIC_DATA;
    pins.data_out_num = I2S_PIN_NO_CHANGE;
    if (i2s_driver_install(I2S_NUM_0, &cfg, 0, nullptr) != ESP_OK) return false;
    if (i2s_set_pin(I2S_NUM_0, &pins) != ESP_OK) return false;
    i2s_stop(I2S_NUM_0);
    return true;
}
bool start() { return i2s_start(I2S_NUM_0) == ESP_OK; }
void stop() { i2s_stop(I2S_NUM_0); }
size_t capture(int16_t* target, size_t capacity, int gain) {
    if (!capacity) return 0;
    int32_t raw[256];
    size_t bytesRead = 0;
    if (i2s_read(I2S_NUM_0, raw, sizeof(raw), &bytesRead, pdMS_TO_TICKS(20)) != ESP_OK) return 0;
    size_t count = min(capacity, bytesRead / sizeof(int32_t));
    for (size_t i = 0; i < count; ++i) {
        int32_t sample = (raw[i] >> 16) * gain;
        if (sample > 32767) sample = 32767;
        if (sample < -32768) sample = -32768;
        target[i] = static_cast<int16_t>(sample);
    }
    return count;
}
}
