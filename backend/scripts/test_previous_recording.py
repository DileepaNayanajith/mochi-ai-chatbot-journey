"""Send a pre-recorded 16k mono WAV to the local server for regression testing."""
import os
import sys
import wave
import urllib.request

if len(sys.argv) != 2:
    raise SystemExit("Usage: python scripts/test_previous_recording.py /path/to/recording.wav")
with wave.open(sys.argv[1], "rb") as file:
    assert (file.getframerate(), file.getnchannels(), file.getsampwidth()) == (16000, 1, 2)
    audio = file.readframes(file.getnframes())
headers = {"Content-Type": "application/octet-stream"}
if os.getenv("MOCHI_API_KEY"):
    headers["X-Mochi-Key"] = os.environ["MOCHI_API_KEY"]
request = urllib.request.Request("http://127.0.0.1:8002/ask", data=audio, headers=headers, method="POST")
with urllib.request.urlopen(request, timeout=30) as result:
    print(result.read().decode("utf-8"))
print("Check /status with scripts/check_server.py for processing completion.")
