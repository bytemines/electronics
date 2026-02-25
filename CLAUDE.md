# CLAUDE.md

## Project
ESP32/Arduino electronics projects. C++ with Arduino framework.

## Build
```bash
arduino-cli compile --fqbn esp32:esp32:esp32:UploadSpeed=115200 <sketch-dir>/
arduino-cli upload --fqbn esp32:esp32:esp32:UploadSpeed=115200 --port /dev/cu.usbserial-110 <sketch-dir>/
```

## Conventions
- One directory per project/sketch
- Web UIs are pure HTML/CSS/JS embedded in `web_page.h` as C strings
- Keep a `preview.html` alongside for local UI testing
- Board: ESP32-D0WD-V3, upload at 115200 baud (921600 fails on this board)
- AP mode by default (SSID: `ESP32-Motor`, pass: `motor1234`, IP: `192.168.4.1`)
