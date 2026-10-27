# Firmware (ESP32-S3)

Open **this folder** as the PlatformIO project in VS Code.
Copy `include/config.example.h` to `include/config.h` and edit locally; `config.h` is intentionally ignored by Git.

The original breadboard prototype used Arduino-ESP32's **legacy `driver/i2s.h`** API, tested under the user's existing PlatformIO toolchain. Later ESP-IDF/Arduino core releases may require I2S migration.

Verified on the user's physical prototype before packaging: ST7789 screen, I2S mic sound detection, 16k PCM upload, MAX98357A 24k playback, and GPIO7 talk button. **This refactored repository version has not been reflashed/retested on the user's board.**
