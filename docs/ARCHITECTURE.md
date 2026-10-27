# Architecture

## Board

ESP32-S3 orchestrates the display, I²S receive from INMP441, I²S transmit to MAX98357A, Wi-Fi client and push button. It does **not** run the language model locally.

## MacBook

`backend/mochi_server.py` loads Whisper (MPS where supported), Kokoro ONNX and an Ollama HTTP client. It maintains one in-flight conversation at a time. Endpoint contracts are defined in [API.md](API.md).

## State machine

`READY -> LISTENING -> UPLOADING -> THINKING -> SPEAKING -> READY` with timeout/error returning to READY. A new question clears the previous reply. The backend generates the entire reply first; there is **no streaming TTS yet**. The ESP32 attempts to download a complete PCM reply before playback to avoid Wi-Fi/I²S underruns.

## Performance

Measured timings are **model processing times**, not full user-perceived end-to-end latency (which also includes upload, polling and playback startup). See [PERFORMANCE.md](PERFORMANCE.md).

## Productization gap

Current prototype has no per-device authentication by default, robust echo cancellation, OTA updates, encrypted transport, privacy-consent UX, data retention or production cloud fleet management. It is not ready to sell as an always-on microphone without those additions and regulatory/security review.
