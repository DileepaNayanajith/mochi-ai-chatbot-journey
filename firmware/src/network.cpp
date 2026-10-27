#include "network.h"
#include <WiFi.h>
#include "config.h"

namespace Network {
bool connect() {
    WiFi.mode(WIFI_STA);
    WiFi.begin(MOCHI_WIFI_SSID, MOCHI_WIFI_PASSWORD);
    const unsigned long start = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - start < 20000) delay(300);
    if (WiFi.status() != WL_CONNECTED) return false;
    Serial.print("ESP32 IP: "); Serial.println(WiFi.localIP());
    return true;
}
bool isConnected() { return WiFi.status() == WL_CONNECTED; }
String url(const char* endpoint) {
    return String("http://") + MOCHI_SERVER_IP + ":" + String(MOCHI_SERVER_PORT) + endpoint;
}
}
