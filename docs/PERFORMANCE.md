# Observed local AI timings

Timings transcribed from the owner's ESP32/MacBook logs for the **fast Qwen3 1.7B** setup. These cover Whisper + Ollama Qwen + Kokoro processing; they are not end-to-end microphone-to-speaker response times.

| User utterance | Whisper (s) | Qwen (s) | Kokoro (s) | Total (s) |
| --- | ---: | ---: | ---: | ---: |
| Can you sing? | 1.28 | 0.60 | 2.17 | 4.05 |
| English song | 0.49 | 0.35 | 1.30 | 2.14 |
| Of course! | 0.45 | 0.46 | 1.92 | 2.84 |
| Play music | 0.52 | 0.39 | 1.39 | 2.30 |
| Tell me a joke | 0.60 | 0.67 | 2.55 | 3.82 |
| Hello, how are you? | 0.89 | 0.75 | 5.05 | 6.68 |
| Who is the Prime Minister of Sri Lanka? | 0.76 | 0.55 | 2.53 | 3.85 |
| Thank you | 0.53 | 0.73 | 2.70 | 3.96 |

Earlier Qwen3 4B build: 4.87 s Whisper, 13.91 s LLM, 5.50 s Kokoro, 24.29 s total on one test question. These are **measurements**, not promised latencies. Response length and warm-up materially affect timings. A current-facts question produced an incorrect office-holder, reinforcing that unconnected local LLMs must not be treated as authoritative live data.

Future optimization directions: instrument HTTP upload/download and first-audio latency; synthesize first sentence asynchronously; stream compressed or chunked PCM; add bounded jitter buffer and keep drawing outside the audio-critical task.
