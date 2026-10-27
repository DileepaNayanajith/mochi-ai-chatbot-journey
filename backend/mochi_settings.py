"""Environment-driven local configuration; no user secrets checked into Git."""
from pathlib import Path
import os

BASE = Path(__file__).resolve().parent
HOST = os.getenv("MOCHI_HOST", "0.0.0.0")
PORT = int(os.getenv("MOCHI_PORT", "8002"))
WHISPER_MODEL = os.getenv("MOCHI_WHISPER_MODEL", "openai/whisper-base.en")
OLLAMA_MODEL = os.getenv("MOCHI_OLLAMA_MODEL", "qwen3:1.7b")
OLLAMA_URL = os.getenv("MOCHI_OLLAMA_URL", "http://127.0.0.1:11434/api/chat")
VOICE = os.getenv("MOCHI_VOICE", "af_sarah")
API_KEY = os.getenv("MOCHI_API_KEY", "")
KOKORO_MODEL = BASE / "kokoro-v1.0.onnx"
KOKORO_VOICES = BASE / "voices-v1.0.bin"
