# Experimental wake-word track — NOT IN THE WORKING FIRMWARE

The conversation explored **“Hey Mochi”** hands-free activation and considered offline Vosk on the MacBook. A **recorded-audio Vosk recognizer test** was proposed, but no positive Vosk output or integrated hands-free loop was supplied. The current device uses **GPIO7 push-to-talk**.

Suggested experimental test:

1. Record an English 16 kHz mono WAV saying only “Hey Mochi”.
2. Install `vosk` into an isolated environment and obtain `vosk-model-small-en-us-0.15` from https://alphacephei.com/vosk/models.
3. Run `test_recording.py` with the WAV path and model directory.
4. Collect false positives, missed detections and word variants **before** deciding on a wake activation threshold.

Vosk is a speech recognizer, not a dedicated low-power wake-word model. Always-on streaming has substantial privacy and reliability implications. Commercial versions should prefer on-device wake detection when practicable.
