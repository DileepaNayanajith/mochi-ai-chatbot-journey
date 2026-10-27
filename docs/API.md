# Local HTTP API (compatible with breadboard prototype)

Base URL: `http://<MacBook-LAN-IP>:8002`. Default no authentication; an optional `X-Mochi-Key` header can be enabled via `MOCHI_API_KEY`. LAN-only.

| Method | Path | Request | Success response |
| --- | --- | --- | --- |
| `GET` | `/health` | none | `200 {"status":"ok","service":"mochi"}` |
| `POST` | `/ask` | application/octet-stream, mono signed PCM16 LE 16 kHz, 0.1–20s | `202 {"status":"processing"}` |
| `GET` | `/status` | none | `200 {"status":"processing\|ready\|error", "phase":"...", "user":"...", "reply":"...", "timings":{...}}` |
| `GET` | `/reply.pcm` | none | `200` binary signed PCM16 LE 24 kHz mono; `409` if not ready |

Only one query in flight. A second `/ask` while processing returns `409`. The result belongs to the **latest single request** (no multi-device IDs). Actual smart-home commands, wake word and audio stream chunks are **not** currently included. `GET /status` is polled roughly every 800 ms by the firmware.
