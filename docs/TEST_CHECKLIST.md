# Hardware acceptance checklist (run after extracting this refactor)

- [ ] `pio run -d firmware` builds under the same Arduino-ESP32 legacy I²S toolchain used in development.
- [ ] Serial Monitor on USB CDC shows boot and MacBook IP.
- [ ] 240×240 display uses correct 180° orientation and green eyes.
- [ ] INMP441 left-channel audio is clear in `last_question.wav`.
- [ ] GPIO7 button starts/stops recording with debounce.
- [ ] `/ask` HTTP 202 then `/status` ready; no secret leakage in logs.
- [ ] 24 kHz Kokoro audio downloads; speaker volume is clear without dropouts.
- [ ] 10 consecutive conversational cycles return to READY.
- [ ] Long/failed request shows ERROR then recovers.
- [ ] Speaker tests performed at safe volume and with correctly rated power supply.

Unchecked = not retested on physical board by the repository packaging process.
