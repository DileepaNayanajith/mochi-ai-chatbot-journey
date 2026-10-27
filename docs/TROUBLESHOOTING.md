# Troubleshooting

| Symptom | Check |
| --- | --- |
| Serial monitor only ESP-ROM boot lines | `-DARDUINO_USB_CDC_ON_BOOT=1` and `-DARDUINO_USB_MODE=1`; re-upload, monitor correct `/dev/cu.usbmodem*` port, press reset |
| No ESP32 connection to Mac | Mac's LAN IP (not ESP32 IP), `mochi_server.py` running on 8002, firewall permission, both clients on same non-isolated network |
| No microphone speech | INMP441 L/R to GND for left-channel read, I²S GPIO4/5/6, 3.3V, inspect `last_question.wav` |
| TFT black | Confirm GMT130-V1.0 ST7789 pinout, 3.3 V, SPI mode 3, reset/DC lines; start with slow SPI for diagnostics |
| Sound too quiet | Check MAX98357A VIN voltage, module gain strap, correct speaker impedance, audio software volume; power off before rewiring |
| Voice stutters | Do not redraw TFT during playback; ensure enough heap/PSRAM to buffer `/reply.pcm`; avoid weak Wi-Fi |
| Firmware says insufficient reply buffer | Enable PSRAM where board supports it, use shorter Qwen answers, or add a dedicated producer/consumer streaming buffer |
| AI replies are outdated | Qwen is offline and cannot verify current news or positions; do not advertise live accuracy |
| API error 401 | Optional `MOCHI_API_KEY` on Mac and firmware must match |
| Python `ModuleNotFoundError` | Activate correct `.venv`; run `python -m pip install -r requirements.txt` |
| Broken code after paste | `python -m py_compile mochi_server.py`; use Git to restore exact file rather than editing massive pasted snippets |

To list meaningful project commits: `git log --oneline --reverse`.
