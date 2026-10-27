# Device configuration

1. `cp firmware/include/config.example.h firmware/include/config.h`
2. Edit SSID, Wi-Fi password, MacBook IP and optional shared key.
3. Start `backend/mochi_server.py`, allow local incoming connections, then upload firmware.
4. Both devices must reach one another on the same trusted LAN; guest network / mobile-hotspot isolation can block requests.

Never upload real passwords, recordings or model weights to GitHub. The optional shared header key is **not a substitute** for TLS or a production device identity system.
