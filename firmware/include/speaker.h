#pragma once
#include <Arduino.h>
namespace Speaker {
    constexpr uint32_t SAMPLE_RATE = 24000;
    bool begin();
    bool playMonoPcm(const uint8_t* data, size_t bytes, float volume = .60f);
}
