# Wiring and power

**ESP32-S3 Dev board** wired to these tested prototype GPIO choices. Disconnect USB power before rewiring.

| Component | Pin | ESP32-S3 |
| --- | --- | --- |
| GMT130-V1.0 240×240 ST7789 IPS | SCK | GPIO12 |
| Display | SDA / MOSI | GPIO11 |
| Display | RES / RST | GPIO10 |
| Display | DC | GPIO9 |
| Display | GND | GND |
| Display | VCC | **3.3 V**, only if the particular board rating agrees |
| Display | BLK | Use the previously verified backlight wiring for the exact module |
| INMP441 microphone | SCK | GPIO4 |
| INMP441 | WS | GPIO5 |
| INMP441 | SD | GPIO6 |
| INMP441 | L/R | GND (left channel) |
| INMP441 | VDD | 3.3 V |
| INMP441 | GND | GND |
| MAX98357A | BCLK | GPIO15 |
| MAX98357A | LRC | GPIO16 |
| MAX98357A | DIN | GPIO17 |
| MAX98357A | VIN | 5 V (if module supports it) |
| MAX98357A | GND | common GND |
| MAX98357A | SPK+ / SPK− | speaker terminals, **not GND** |
| Talk button | one side | GPIO7 |
| Talk button | other side | GND |

`INPUT_PULLUP` is used for the button. The 240×240 display is rotated 180° (`setRotation(2)`) and uses SPI mode 3, matching the user's successful display experiment. A `GAIN` strap changes MAX98357A amplifier gain; follow the exact module datasheet, start with low software volume, and power off before changing straps. Do not short speaker terminals to ground.
