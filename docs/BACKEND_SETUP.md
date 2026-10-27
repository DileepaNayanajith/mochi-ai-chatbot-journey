# MacBook backend setup (English)

Prerequisites: **Python 3.11** and Ollama available locally. MacBook and ESP32 need to be on the same trusted Wi-Fi. The project owner originally tested on an Apple Silicon Mac using `mps`.

```bash
cd backend
python3.11 -m venv .venv
source .venv/bin/activate
python -m pip install -U pip
python -m pip install -r requirements.txt
ollama pull qwen3:1.7b
open -a Ollama
```

Get Kokoro model files from the upstream **model-files-v1.1** release (not stored in this repo):

```bash
curl -fL -o kokoro-v1.0.onnx https://github.com/thewh1teagle/kokoro-onnx/releases/download/model-files-v1.1/kokoro-v1.0.onnx
curl -fL -o voices-v1.0.bin https://github.com/thewh1teagle/kokoro-onnx/releases/download/model-files-v1.1/voices-v1.0.bin
```

Optional local access key (must match `MOCHI_API_KEY` in `firmware/include/config.h`):

```bash
export MOCHI_API_KEY='choose-a-private-local-key'
```

Start server in a dedicated Terminal:

```bash
python mochi_server.py
```

In another Terminal:

```bash
curl http://127.0.0.1:8002/health
# If API key enabled:
curl -H "X-Mochi-Key: $MOCHI_API_KEY" http://127.0.0.1:8002/status
```

To replay a real recording without touching the ESP32:

```bash
python scripts/test_previous_recording.py /absolute/path/to/recording.wav
python scripts/check_server.py
```

Use `ipconfig getifaddr en0` on Mac to find its current Wi-Fi LAN IP; network interfaces vary. Set that IP in the ESP32 config. `mochi_server.py` generates `last_question.wav` and `mochi_live_reply.wav` in the backend directory; they are ignored by Git. Quit with `Ctrl+C`.

The default HTTP server listens on `0.0.0.0:8002`. Firewall rules and guest-network isolation can prevent ESP32 access. Do not expose it to the Internet.
