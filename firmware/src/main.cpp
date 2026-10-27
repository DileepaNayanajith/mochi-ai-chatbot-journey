#include <Arduino.h>
#include <esp_heap_caps.h>
#include "face.h"
#include "microphone.h"
#include "speaker.h"
#include "talk_button.h"
#include "network.h"
#include "transport.h"

namespace {
constexpr size_t MAX_SECONDS = 5;
constexpr size_t MAX_SAMPLES = Microphone::SAMPLE_RATE * MAX_SECONDS;
int16_t* recorded = nullptr;
size_t count = 0;
RobotState state = RobotState::READY;

void setState(RobotState next) { state = next; Face::setState(next); }

void recordStart() {
    count = 0;
    if (!Microphone::start()) { setState(RobotState::ERROR_STATE); return; }
    setState(RobotState::LISTENING);
    Serial.println("RECORDING STARTED");
}

void recordStopAndAsk() {
    if (state != RobotState::LISTENING) return;
    Microphone::stop();
    Serial.printf("Recording: %.2f seconds\n", float(count)/Microphone::SAMPLE_RATE);
    if (count < 3200) { setState(RobotState::READY); return; }
    setState(RobotState::UPLOADING);
    if (!Transport::uploadQuestion(recorded, count)) {
        Serial.println("Upload failed");
        setState(RobotState::ERROR_STATE);
        delay(800); setState(RobotState::READY); return;
    }
    setState(RobotState::THINKING);
    const unsigned long start = millis();
    bool ready = false;
    while (millis() - start < 180000) {
        String reason;
        Transport::ReplyState result = Transport::getStatus(reason);
        if (result == Transport::ReplyState::READY) { ready = true; break; }
        if (result == Transport::ReplyState::ERROR) { Serial.println(reason); break; }
        Face::tick();
        delay(800);
    }
    if (!ready) {
        Serial.println("AI reply unavailable");
        setState(RobotState::ERROR_STATE);
        delay(1000); setState(RobotState::READY); return;
    }
    setState(RobotState::SPEAKING);
    // During playback Face::tick is deliberately disabled; prevent dropouts.
    const bool played = Transport::fetchAndPlayReply();
    Serial.println(played ? "VOICE PLAYBACK COMPLETE" : "VOICE PLAYBACK FAILED");
    if (!played) { setState(RobotState::ERROR_STATE); delay(800); }
    setState(RobotState::READY);
}
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("PRINTPRO3D / MOCHI AI Chatbot Journey");
    TalkButton::begin();
    Face::begin();
    const size_t bytes = MAX_SAMPLES * sizeof(int16_t);
    if (psramFound()) recorded = (int16_t*)heap_caps_malloc(bytes, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    if (!recorded) recorded = (int16_t*)heap_caps_malloc(bytes, MALLOC_CAP_8BIT);
    if (!recorded) { Serial.println("No audio recording memory"); setState(RobotState::ERROR_STATE); return; }
    if (!Microphone::begin()) { Serial.println("Mic setup failed"); setState(RobotState::ERROR_STATE); return; }
    if (!Speaker::begin()) { Serial.println("Speaker setup failed"); setState(RobotState::ERROR_STATE); return; }
    if (!Network::connect()) { Serial.println("Wi-Fi setup failed"); setState(RobotState::ERROR_STATE); return; }
    setState(RobotState::READY);
}

void loop() {
    if (state == RobotState::ERROR_STATE) { delay(100); return; }
    const auto event = TalkButton::poll();
    if (event == TalkButton::Event::PRESSED && state == RobotState::READY) recordStart();
    if (event == TalkButton::Event::RELEASED && state == RobotState::LISTENING) recordStopAndAsk();
    if (state == RobotState::LISTENING) {
        count += Microphone::capture(recorded + count, MAX_SAMPLES - count);
        if (count >= MAX_SAMPLES) recordStopAndAsk();
    }
    Face::tick();
    delay(5);
}
