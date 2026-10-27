"""Small standard-library audio/protocol helpers shared with unit tests."""
import re
import wave
from pathlib import Path

MIC_RATE = 16000
VOICE_RATE = 24000
MAX_RECORD_SECONDS = 20

def validate_pcm(payload: bytes) -> bool:
    return isinstance(payload, bytes) and 3200 <= len(payload) <= MIC_RATE*2*MAX_RECORD_SECONDS and len(payload) % 2 == 0

def clean_for_tts(text: str) -> str:
    # Avoid speaking emojis, markup and repeated whitespace.
    text = re.sub(r"[\U0001F000-\U0001FAFF\u2600-\u27BF]", "", text)
    text = re.sub(r"[`*_#]+", "", text)
    return " ".join(text.split())

def save_wav(path: Path, pcm: bytes, sample_rate: int):
    with wave.open(str(path), "wb") as wav:
        wav.setnchannels(1)
        wav.setsampwidth(2)
        wav.setframerate(sample_rate)
        wav.writeframes(pcm)
