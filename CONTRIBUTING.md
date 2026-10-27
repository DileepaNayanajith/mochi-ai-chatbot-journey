# Contributing

Changes should build on the working ESP32-S3 prototype rather than replacing known-good hardware behavior without a test.

1. Create a short-lived feature branch.
2. Never commit Wi-Fi credentials, IP-specific config, voice recordings, model weights, or API keys.
3. Run `cd backend && python3 -m unittest discover -s tests -v`.
4. Build firmware with PlatformIO and manually test on actual hardware.
5. Describe hardware and software test results accurately in pull requests.

The commit history in this archive is a **retrospective reconstruction of project milestones**, not a claim that these commits were authored on the dates of the original experiments. Commit timestamps are generated when this repository was packaged.
