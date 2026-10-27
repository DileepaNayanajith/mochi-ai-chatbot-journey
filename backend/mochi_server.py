"""PRINTPRO3D Mochi local assistant: /ask, /status and /reply.pcm.

Designed for one ESP32-S3 on a trusted local network. Not a public API.
"""
import json
import threading
import time
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer

from mochi_settings import (
    HOST, PORT, API_KEY, BASE, WHISPER_MODEL, OLLAMA_MODEL,
    OLLAMA_URL, VOICE, KOKORO_MODEL, KOKORO_VOICES,
)
from voice_core import MIC_RATE, validate_pcm, save_wav
from mochi_stt import Recognizer
from mochi_brain import Brain
from mochi_tts import Voice

# Load models once, not on every question.
print("MOCHI AI Chatbot Journey — local server", flush=True)
print("Loading Whisper", WHISPER_MODEL, flush=True)
stt = Recognizer(WHISPER_MODEL)
print("Loading Kokoro", flush=True)
tts = Voice(KOKORO_MODEL, KOKORO_VOICES, VOICE)
brain = Brain(OLLAMA_MODEL, OLLAMA_URL)
print("AI models ready; Ollama model:", OLLAMA_MODEL, flush=True)

lock = threading.Lock()
state = {"status": "idle", "phase": "idle", "user": "", "reply": "", "error": "", "timings": {}}
reply_pcm = b""

def set_state(**changes):
    with lock:
        state.update(changes)

def process_audio(raw):
    global reply_pcm
    start = time.perf_counter()
    try:
        set_state(phase="whisper")
        save_wav(BASE / "last_question.wav", raw, MIC_RATE)
        user_text, stt_seconds = stt.transcribe(raw)
        print("YOU:", user_text, "| whisper", stt_seconds, "s", flush=True)

        set_state(phase="qwen", user=user_text)
        answer, llm_seconds = brain.reply(user_text)
        print("MOCHI:", answer, "| qwen", llm_seconds, "s", flush=True)

        set_state(phase="kokoro", reply=answer)
        pcm, tts_seconds = tts.synthesize(answer, BASE / "mochi_live_reply.wav")
        timings = {
            "whisper": stt_seconds,
            "qwen": llm_seconds,
            "kokoro": tts_seconds,
            "total": round(time.perf_counter()-start, 2),
        }
        with lock:
            reply_pcm = pcm
            state.update(status="ready", phase="complete", user=user_text, reply=answer, error="", timings=timings)
        print("PERFORMANCE:", json.dumps(timings), flush=True)
    except Exception as exc:
        print("PROCESSING ERROR:", repr(exc), flush=True)
        set_state(status="error", phase="error", error=str(exc))

class Handler(BaseHTTPRequestHandler):
    def _authorized(self):
        if not API_KEY:
            return True
        # Header token is only a basic local safeguard. Use TLS for Internet deployment.
        import hmac
        return hmac.compare_digest(self.headers.get("X-Mochi-Key", ""), API_KEY)

    def send_json(self, code, payload):
        data = json.dumps(payload, ensure_ascii=False).encode("utf-8")
        self.send_response(code)
        self.send_header("Content-Type", "application/json; charset=utf-8")
        self.send_header("Content-Length", str(len(data)))
        self.end_headers()
        self.wfile.write(data)

    def do_GET(self):
        if self.path == "/health":
            self.send_json(200, {"status": "ok", "service": "mochi"})
            return
        if not self._authorized():
            self.send_json(401, {"error": "unauthorized"})
            return
        if self.path == "/status":
            with lock:
                response = dict(state)
            self.send_json(200, response)
            return
        if self.path == "/reply.pcm":
            with lock:
                ready = state["status"] == "ready"
                pcm = reply_pcm
            if not ready:
                self.send_json(409, {"error": "reply not ready"})
                return
            self.send_response(200)
            self.send_header("Content-Type", "application/octet-stream")
            self.send_header("Content-Length", str(len(pcm)))
            self.end_headers()
            try:
                self.wfile.write(pcm)
            except (BrokenPipeError, ConnectionResetError):
                print("ESP32 disconnected mid-reply", flush=True)
            return
        self.send_error(404)

    def do_POST(self):
        global reply_pcm
        if self.path != "/ask":
            self.send_error(404)
            return
        if not self._authorized():
            self.send_json(401, {"error": "unauthorized"})
            return
        try:
            length = int(self.headers.get("Content-Length", "0"))
        except ValueError:
            self.send_json(400, {"error": "invalid Content-Length"})
            return
        # Validate before loading unbounded request bodies into memory.
        if not 3200 <= length <= MIC_RATE * 2 * 20 or length % 2:
            self.send_json(400, {"error": "invalid audio length"})
            return
        with lock:
            if state["status"] == "processing":
                busy = True
            else:
                busy = False
                state.update(status="processing", phase="upload", user="", reply="", error="", timings={})
                reply_pcm = b""
        if busy:
            self.send_json(409, {"error": "Mochi is busy"})
            return
        try:
            self.connection.settimeout(30)
            raw = self.rfile.read(length)
            if len(raw) != length or not validate_pcm(raw):
                raise ValueError("incomplete or malformed raw PCM")
        except Exception as exc:
            set_state(status="error", phase="error", error=str(exc))
            self.send_json(400, {"error": str(exc)})
            return
        threading.Thread(target=process_audio, args=(raw,), daemon=True).start()
        self.send_json(202, {"status": "processing"})

    def log_message(self, fmt, *args):
        message = fmt % args
        if "GET /status" not in message:
            print(message, flush=True)

if __name__ == "__main__":
    server = ThreadingHTTPServer((HOST, PORT), Handler)
    server.daemon_threads = True
    print(f"MOCHI SERVER READY on http://{HOST}:{PORT}", flush=True)
    try:
        server.serve_forever()
    except KeyboardInterrupt:
        pass
    finally:
        server.server_close()
