"""Optional offline WAV phrase experiment; not wired to ESP32 firmware."""
import json
import sys
import wave

if len(sys.argv) != 3:
    raise SystemExit("Usage: python test_recording.py path/to/hey_mochi.wav path/to/vosk_model")
from vosk import Model, KaldiRecognizer, SetLogLevel
SetLogLevel(-1)

with wave.open(sys.argv[1], "rb") as f:
    if (f.getframerate(), f.getnchannels(), f.getsampwidth()) != (16000, 1, 2):
        raise ValueError("Expected 16 kHz 16-bit mono PCM WAV")
    recognizer = KaldiRecognizer(Model(sys.argv[2]), 16000)
    parts = []
    while data := f.readframes(4000):
        if recognizer.AcceptWaveform(data):
            parts.append(json.loads(recognizer.Result()).get("text", ""))
    parts.append(json.loads(recognizer.FinalResult()).get("text", ""))

heard = " ".join(parts).strip().lower()
print("Recognized:", heard)
print("Wake phrase exact match:", "hey mochi" in heard)
