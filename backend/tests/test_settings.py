import unittest
from voice_core import MIC_RATE, VOICE_RATE, MAX_RECORD_SECONDS

class FormatContractTests(unittest.TestCase):
    def test_recording_contract(self):
        self.assertEqual(MIC_RATE, 16000)
        self.assertEqual(VOICE_RATE, 24000)
        self.assertEqual(MAX_RECORD_SECONDS, 20)

if __name__ == "__main__":
    unittest.main()
