"""Local Ollama Qwen responder; avoids claiming nonexistent integrations."""
import json
import time
import urllib.request

SYSTEM_PROMPT = """You are Mochi, a friendly English-speaking AI companion robot built by PRINTPRO3D.
Reply naturally in one brief spoken sentence (prefer under 25 words).
No emoji, markdown, or unnecessary repetition.
You have no tools connected: do not claim to have played music, searched the web,
controlled home devices, or performed other actions. Say honestly if unavailable.
For live current facts, tell the user you cannot verify live data yet.
"""

class Brain:
    def __init__(self, model: str, url: str):
        self.model = model
        self.url = url

    def reply(self, prompt: str):
        start = time.perf_counter()
        payload = {
            "model": self.model,
            "stream": False,
            "think": False,
            "keep_alive": "30m",
            "messages": [
                {"role": "system", "content": SYSTEM_PROMPT},
                {"role": "user", "content": prompt},
            ],
            "options": {"temperature": 0.5, "repeat_penalty": 1.15,
                        "num_predict": 50, "num_ctx": 2048},
        }
        request = urllib.request.Request(
            self.url,
            data=json.dumps(payload).encode("utf-8"),
            headers={"Content-Type": "application/json"},
            method="POST",
        )
        with urllib.request.urlopen(request, timeout=180) as response:
            answer = json.load(response)
        text = answer.get("message", {}).get("content", "").strip()
        if not text: raise ValueError("AI returned empty reply")
        return text, round(time.perf_counter() - start, 2)
