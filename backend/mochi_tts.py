"""Kokoro ONNX generation: 24 kHz mono signed PCM16."""
import time
import numpy as np
from voice_core import clean_for_tts, save_wav, VOICE_RATE

class Voice:
    def __init__(self, model_path, voice_path, voice_name="af_sarah"):
        from kokoro_onnx import Kokoro
        self.engine = Kokoro(str(model_path), str(voice_path))
        self.voice_name = voice_name

    def synthesize(self, text, output_path):
        start = time.perf_counter()
        clean = clean_for_tts(text)
        if not clean: raise ValueError("No TTS text")
        samples, rate = self.engine.create(clean, voice=self.voice_name, speed=1.0, lang="en-us")
        if rate != VOICE_RATE: raise ValueError(f"Unexpected TTS sample rate: {rate}")
        samples = np.asarray(samples, dtype=np.float32)
        if samples.ndim != 1: raise ValueError("Expected mono output")
        pcm = (np.clip(samples, -1, 1) * 32767).astype("<i2").tobytes()
        save_wav(output_path, pcm, rate)
        return pcm, round(time.perf_counter() - start, 2)
