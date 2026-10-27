#include "speaker.h"
#include "board_pins.h"
#include <driver/i2s.h>

namespace Speaker {
bool begin() {
    i2s_config_t cfg = {};
    cfg.mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX);
    cfg.sample_rate = SAMPLE_RATE;
    cfg.bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT;
    cfg.channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT;
    cfg.communication_format = I2S_COMM_FORMAT_STAND_I2S;
    cfg.dma_buf_count = 8;
    cfg.dma_buf_len = 512;
    cfg.use_apll = false;
    cfg.tx_desc_auto_clear = true;
    i2s_pin_config_t pins = {};
    pins.mck_io_num = I2S_PIN_NO_CHANGE;
    pins.bck_io_num = Pins::SPK_BCLK;
    pins.ws_io_num = Pins::SPK_LRC;
    pins.data_out_num = Pins::SPK_DIN;
    pins.data_in_num = I2S_PIN_NO_CHANGE;
    if (i2s_driver_install(I2S_NUM_1, &cfg, 0, nullptr) != ESP_OK) return false;
    return i2s_set_pin(I2S_NUM_1, &pins) == ESP_OK;
}
bool playMonoPcm(const uint8_t* data, size_t bytes, float volume) {
    if (!data || !bytes || (bytes & 1)) return false;
    const float gain = constrain(volume, 0.0f, 1.0f);
    int16_t stereo[1024];
    i2s_zero_dma_buffer(I2S_NUM_1);
    for (size_t offset = 0; offset < bytes;) {
        const size_t monoSamples = min(size_t(512), (bytes - offset) / 2);
        for (size_t i = 0; i < monoSamples; ++i) {
            const uint16_t u = uint16_t(data[offset + 2*i]) | (uint16_t(data[offset + 2*i+1]) << 8);
            const int16_t value = static_cast<int16_t>(u);
            const int16_t output = static_cast<int16_t>(value * gain);
            stereo[2*i] = output;
            stereo[2*i + 1] = output;
        }
        size_t written = 0;
        const size_t outBytes = monoSamples * 2 * sizeof(int16_t);
        if (i2s_write(I2S_NUM_1, stereo, outBytes, &written, portMAX_DELAY) != ESP_OK || written != outBytes) return false;
        offset += monoSamples * 2;
    }
    // Wait for queued DMA frames before clearing; no TFT drawing occurs during output.
    delay(300);
    i2s_zero_dma_buffer(I2S_NUM_1);
    return true;
}
}
