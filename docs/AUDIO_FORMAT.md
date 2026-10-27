# End-to-end audio format

1. ESP32 INMP441 samples 32-bit I2S left channel at **16,000 samples/s**.
2. Firmware right-shifts and clamps to signed **16-bit little-endian mono PCM**; it posts raw bytes to `/ask`.
3. Server writes `last_question.wav` for local debugging and transcribes using Whisper Base English.
4. Qwen generates text via Ollama; Kokoro renders **24,000 samples/s signed 16-bit little-endian mono PCM**.
5. ESP32 reads `/reply.pcm`, buffers the reply in PSRAM (or available heap), duplicates each sample into stereo I2S, and plays through MAX98357A.

Reason for static eyes while talking: earlier prototype had audible dropouts when the SPI display was frequently redrawn in the same loop. The refactored firmware prioritizes audio playback over eye frames. **Buffered playback is an improvement but needs physical re-test**. Longer replies may fail if RAM cannot accommodate them; then shorten replies, enable PSRAM or implement a ring-buffer/dual-task streaming design.
