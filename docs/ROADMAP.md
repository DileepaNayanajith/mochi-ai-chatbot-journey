# Roadmap (not shipped features)

## Immediate
- [ ] Physically build/test the **modularized** firmware archive on actual board and pin wiring
- [ ] Measure first-audio latency as well as total AI processing
- [ ] Play speaking eye animations without I²S dropouts (FreeRTOS task / ring buffer)
- [ ] Make 5-second recording duration adjustable and add speech endpoint detection (VAD)

## Wake word
- [ ] Benchmark detection of **Hey Mochi** across Sri Lankan accents and background noise
- [ ] Choose server-side Vosk experimental path or on-device custom microWakeWord model **with verified licensing and accuracy**
- [ ] Never send continuous audio off the device without a clear microphone status/consent design
- [ ] After wake detection, use a bounded speech window and explicit timeout

## Assistant capabilities
- [ ] Explicit turn-by-turn memory with opt-in and deletion controls
- [ ] Fast first-sentence Kokoro synthesis and overlapped playback
- [ ] Home Assistant integration with allowlisted entities and command confirmations
- [ ] Actual music playback tool and clear copyright/legal media integration
- [ ] Current-facts lookup with source citations rather than model guesses

## Product candidate
- [ ] Robust 3D-printed enclosure, stable power, better far-field mic/audio echo cancellation
- [ ] Secure Wi-Fi provisioning; unique device identities, OTA updates, safe cloud/on-prem deploy plan
- [ ] Cost, latency, enclosure and hardware endurance testing; privacy and commercial license audit
