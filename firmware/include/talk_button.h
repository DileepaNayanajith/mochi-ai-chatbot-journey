#pragma once
#include <Arduino.h>
namespace TalkButton {
    enum class Event : uint8_t { NONE, PRESSED, RELEASED };
    void begin();
    Event poll();
}
