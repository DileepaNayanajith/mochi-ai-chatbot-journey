#include "transport.h"
#include "network.h"
#include "speaker.h"
#include "config.h"
#include <HTTPClient.h>
#include <WiFiClient.h>
#include <ArduinoJson.h>
#include <esp_heap_caps.h>

namespace {
void authenticate(HTTPClient& http) {
    if (strlen(MOCHI_API_KEY)) http.addHeader("X-Mochi-Key", MOCHI_API_KEY);
}
}
namespace Transport {
bool uploadQuestion(const int16_t* samples, size_t count) {
    if (!Network::isConnected() || !samples || count < 1600) return false;
    WiFiClient client;
    HTTPClient http;
    http.setTimeout(30000);
    if (!http.begin(client, Network::url("/ask"))) return false;
    authenticate(http);
    http.addHeader("Content-Type", "application/octet-stream");
    const int status = http.POST((uint8_t*)samples, count * sizeof(int16_t));
    Serial.printf("Upload HTTP: %d\n", status);
    http.end();
    return status == 202;
}
ReplyState getStatus(String& reason) {
    WiFiClient client;
    HTTPClient http;
    http.setTimeout(10000);
    if (!http.begin(client, Network::url("/status"))) {
        reason = "Cannot reach server";
        return ReplyState::ERROR;
    }
    authenticate(http);
    if (http.GET() != HTTP_CODE_OK) {
        reason = "HTTP /status failed";
        http.end();
        return ReplyState::ERROR;
    }
    const String body = http.getString();
    http.end();
    DynamicJsonDocument doc(2048);
    if (deserializeJson(doc, body)) { reason = "Malformed status JSON"; return ReplyState::ERROR; }
    const String status = doc["status"].as<String>();
    if (status == "ready") return ReplyState::READY;
    if (status == "error") {
        reason = doc["error"].as<String>();
        return ReplyState::ERROR;
    }
    if (status == "processing") return ReplyState::PROCESSING;
    reason = "Unknown status";
    return ReplyState::ERROR;
}
bool fetchAndPlayReply() {
    WiFiClient client;
    HTTPClient http;
    http.setTimeout(15000);
    if (!http.begin(client, Network::url("/reply.pcm"))) return false;
    authenticate(http);
    const int status = http.GET();
    Serial.printf("Voice HTTP: %d\n", status);
    if (status != 200) { http.end(); return false; }
    const int n = http.getSize();
    if (n <= 0 || (n & 1) || n > 2000000) { http.end(); return false; }
    WiFiClient* stream = http.getStreamPtr();
    stream->setTimeout(15000);
    // Buffer whole reply before I2S playback: eliminate Wi-Fi audio starvation.
    // Prefer PSRAM, but small responses may fit in ordinary heap memory.
    uint8_t* buffer = nullptr;
    if (psramFound()) buffer = (uint8_t*)heap_caps_malloc(n, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    if (!buffer) buffer = (uint8_t*)heap_caps_malloc(n, MALLOC_CAP_8BIT);
    if (!buffer) {
        Serial.println("Not enough RAM for voice buffer. Shorten AI reply or enable PSRAM.");
        http.end();
        return false;
    }
    size_t have = 0;
    bool good = true;
    while (have < size_t(n)) {
        const size_t count = min(size_t(2048), size_t(n) - have);
        const size_t got = stream->readBytes(buffer + have, count);
        if (got != count) { good = false; break; }
        have += got;
    }
    http.end();
    if (good) good = Speaker::playMonoPcm(buffer, n, 0.60f);
    heap_caps_free(buffer);
    return good;
}
}
