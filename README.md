# Electronics

Arduino and ESP32 projects.

## Projects

### esp32-motor-controller

Brushless DC motor controller with a web interface served by the ESP32.

**Stack:** C++ (Arduino), pure HTML/CSS/JS, ESP32 WebServer

**How it works:**
```
Browser/Phone  --WiFi-->  ESP32 (web server + PWM)  --signal-->  ESC  -->  BLDC Motor
```

**Wiring:**
| ESP32 Pin | ESC Wire | Function |
|-----------|----------|----------|
| GPIO 13 | Yellow/Orange | PWM signal |
| GND | Brown | Ground |
| — | Red | +5V BEC (leave disconnected if USB powered) |

**Upload:**
```bash
arduino-cli compile --fqbn esp32:esp32:esp32:UploadSpeed=115200 esp32-motor-controller/
arduino-cli upload --fqbn esp32:esp32:esp32:UploadSpeed=115200 --port /dev/cu.usbserial-110 esp32-motor-controller/
```

**Connect:** Join WiFi `ESP32-Motor` (pass: `motor1234`), open `192.168.4.1`

**Preview UI locally:** Open `esp32-motor-controller/preview.html` in a browser.
