"""Whisper recognizer loaded once at backend startup."""
import time
import numpy as np

class Recognizer:
    def __init__(self, model_name: str):
        import torch
        from transformers import pipeline
        device = "mps" if torch.backends.mps.is_available() else "cpu"
        print("Whisper device:", device, flush=True)
        self.asr = pipeline("automatic-speech-recognition", model=model_name, device=device)

    def transcribe(self, raw: bytes):
        start = time.perf_counter()
        audio = np.frombuffer(raw, dtype="<i2").astype(np.float32) / 32768.0
        result = self.asr({"raw": audio, "sampling_rate": 16000})
        text = result["text"].strip()
        if not text:
            raise ValueError("No speech recognized")
        return text, round(time.perf_counter() - start, 2)
