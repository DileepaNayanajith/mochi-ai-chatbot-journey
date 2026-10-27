#pragma once
#include <Arduino.h>
namespace Microphone {
    constexpr uint32_t SAMPLE_RATE = 16000;
    bool begin();
    bool start();
    void stop();
    size_t capture(int16_t* target, size_t capacity, int gain = 2);
}
