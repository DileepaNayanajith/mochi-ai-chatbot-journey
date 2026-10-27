# AI Chatbot Journey — project log

A practical story of repeated bring-up, debugging and audio improvements. Milestones come from the owner's experiment notes and files; dates are **not invented** for individual steps. This repository's commit timestamps are when the retrospective archive was assembled.

1. **Prototype idea:** A cute Sinhala-speaking DIY Mochi companion, later evolving toward an Alexa-style home assistant.
2. **Display bring-up:** 240×240 ST7789/GMT130-V1.0 IPS works with SPI mode 3; the screen was physically rotated 180° via `setRotation(2)`.
3. **Face prototype:** Simple expressive eye tests inspired by the *idea* of RoboEyes. The project eventually chose two rounded green eyes as its normal face, with emotions reserved for later AI-driven behavior.
4. **Audio hardware:** INMP441 microphone levels became visible over USB Serial. MAX98357A amplifier made a memorable loud beep; the output volume and module gain were then adjusted.
5. **Wi-Fi bridge:** ESP32 recorded and uploaded 16 kHz mono PCM; MacBook saved a playable WAV. An initial Mac connection failure was solved by using the correct Mac LAN IP and running server.
6. **Sinhala experiment:** Local fine-tuned Whisper detected the test phrase, while a small Qwen model repeated Sinhala words. The prototype pivoted to English to validate fluent dialogue first.
7. **English STT:** Whisper Base English transcribed the recorded “How are you?” test accurately.
8. **Local AI:** Qwen3 4B via Ollama generated natural English text; Kokoro ONNX synthesized a female English voice.
9. **Speaker integration:** 24 kHz PCM streamed to ESP32 and played through MAX98357A, with the talking face displayed.
10. **Button conversation:** GPIO7 push-to-talk evolved into full automatic cycle. Original working backend used `/ask`, `/status`, and `/reply.pcm`.
11. **Audio repair:** Sound became choppy when full TFT speaking animations were drawn during playback; audio stabilized after those frame updates were disabled.
12. **Performance tuning:** Qwen3 1.7B in non-thinking mode significantly reduced model processing time. Some recorded questions finished in 2–4 seconds (model stages only), with longer replies spending more time in Kokoro.
13. **Wake word concept:** “Hey Mochi” and Vosk were *proposed/test planning*, not verified as a full hands-free experience. The shipped prototype remains push-to-talk.

## What this archive represents

**A reconstructed, readable repository from a successful prototype journey.** It includes an implementation derived from the user's working sketches, hardware photos provided by the owner, setup documentation and a real Git history created for this package. It does not forge original commit dates, pretend the new refactor has been hardware-tested, or add generated/fake project photography.
