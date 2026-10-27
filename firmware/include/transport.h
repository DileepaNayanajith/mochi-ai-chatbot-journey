#pragma once
#include <Arduino.h>
namespace Transport {
    enum class ReplyState { PROCESSING, READY, ERROR };
    bool uploadQuestion(const int16_t* samples, size_t count);
    ReplyState getStatus(String& reason);
    bool fetchAndPlayReply();
}
