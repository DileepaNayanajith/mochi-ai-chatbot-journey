"""Basic live health and status check; no AI question is uploaded."""
import json
import os
import urllib.request

host = os.environ.get("MOCHI_CHECK_URL", "http://127.0.0.1:8002")
key = os.environ.get("MOCHI_API_KEY", "")
for path in ("/health", "/status"):
    headers = {"X-Mochi-Key": key} if key else {}
    request = urllib.request.Request(host + path, headers=headers)
    with urllib.request.urlopen(request, timeout=5) as reply:
        print(path, json.dumps(json.load(reply), indent=2))
