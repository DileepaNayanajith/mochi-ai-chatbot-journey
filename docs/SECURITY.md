# Security, privacy and production limitations

This repository contains neither home Wi-Fi credentials nor microphone recordings nor AI model files. The original firmware and backend are local-network prototypes. By default, the HTTP API is **unauthenticated** and uses **unencrypted** HTTP. Optional `MOCHI_API_KEY` with `X-Mochi-Key` helps prevent casual accidental use on a trusted home LAN, but does not make the API secure on public networks.

**Do not expose port 8002 to the public Internet.** Production needs TLS termination, unique device credentials, rate limits, request IDs, endpoint isolation, OTA update signing, consent/recording indicators, controls for deleting user recordings and a written retention policy. Never perform mains-power home automation without certified hardware and safety review.

`last_question.wav` and `mochi_live_reply.wav` are local debug files retained until manually deleted; leave them out of GitHub and avoid storing other people's voice recordings without consent. The always-listening wake-word mode is **not implemented** here.
