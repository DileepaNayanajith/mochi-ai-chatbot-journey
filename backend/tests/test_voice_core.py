import io
import tempfile
import unittest
import wave
from pathlib import Path
from voice_core import validate_pcm, clean_for_tts, save_wav

class VoiceCoreTests(unittest.TestCase):
    def test_valid_short_recording(self):
        self.assertTrue(validate_pcm(b"\0" * 3200))
    def test_odd_byte_count_rejected(self):
        self.assertFalse(validate_pcm(b"\0" * 3201))
    def test_large_recording_rejected(self):
        self.assertFalse(validate_pcm(b"\0" * 641000))
    def test_cleanup_text(self):
        self.assertEqual(clean_for_tts("Hi! 😊  **How** are you?"), "Hi! How are you?")
    def test_wav_header(self):
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp) / "a.wav"
            save_wav(path, b"\0" * 3200, 16000)
            with wave.open(str(path), "rb") as f:
                self.assertEqual(f.getframerate(), 16000)
                self.assertEqual(f.getnchannels(), 1)
                self.assertEqual(f.getsampwidth(), 2)

if __name__ == "__main__":
    unittest.main()
