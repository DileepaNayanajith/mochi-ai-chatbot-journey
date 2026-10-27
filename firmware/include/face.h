#pragma once
#include <Arduino.h>

enum class RobotState : uint8_t {
    READY, LISTENING, UPLOADING, THINKING, SPEAKING, ERROR_STATE
};

namespace Face {
    void begin();
    void setState(RobotState state);
    void tick();
}
