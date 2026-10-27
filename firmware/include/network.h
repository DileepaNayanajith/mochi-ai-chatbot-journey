#pragma once
#include <Arduino.h>
namespace Network {
    bool connect();
    bool isConnected();
    String url(const char* endpoint);
}
