#include "face.h"
#include "board_pins.h"
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <math.h>

namespace {
Adafruit_ST7789 tft(&SPI, -1, Pins::TFT_DC, Pins::TFT_RESET);
RobotState currentState = RobotState::READY;
unsigned long lastFrame = 0;
const char* label(RobotState s) {
    switch (s) {
        case RobotState::READY: return "READY";
        case RobotState::LISTENING: return "LISTENING";
        case RobotState::UPLOADING: return "UPLOADING";
        case RobotState::THINKING: return "THINKING";
        case RobotState::SPEAKING: return "SPEAKING";
        default: return "ERROR";
    }
}
void render() {
    const unsigned long now = millis();
    float openness = 1.0f;
    if (currentState == RobotState::READY) {
        const unsigned long phase = now % 3500;
        if (phase < 180) {
            const float p = phase / 180.0f;
            openness = .08f + .92f * fabsf(2.0f * p - 1.0f);
        }
    }
    const int h = max(6, int(78 * openness));
    const int radius = min(19, h / 2);
    tft.fillRect(25, 60, 190, 135, ST77XX_BLACK);
    tft.fillRoundRect(37, 113 - h/2, 76, h, radius, ST77XX_GREEN);
    tft.fillRoundRect(127, 113 - h/2, 76, h, radius, ST77XX_GREEN);
    tft.fillRect(20, 205, 200, 20, ST77XX_BLACK);
    tft.setTextSize(1);
    tft.setTextColor(ST77XX_WHITE);
    int16_t x1, y1;
    uint16_t w, ht;
    tft.getTextBounds(label(currentState), 0, 0, &x1, &y1, &w, &ht);
    tft.setCursor((240 - w) / 2, 210);
    tft.print(label(currentState));
}
}
namespace Face {
void begin() {
    SPI.begin(Pins::TFT_SCK, -1, Pins::TFT_MOSI, -1);
    tft.init(240, 240, SPI_MODE3);
    tft.setSPISpeed(24000000);
    tft.setRotation(2);
    tft.fillScreen(ST77XX_BLACK);
    render();
}
void setState(RobotState s) {
    currentState = s;
    Serial.print("STATE: "); Serial.println(label(s));
    render();
    lastFrame = millis();
}
void tick() {
    // CRITICAL: No TFT writes while speaking. Those caused I2S dropouts on the prototype.
    if (currentState == RobotState::SPEAKING) return;
    if (millis() - lastFrame < 100) return;
    lastFrame = millis();
    render();
}
}
