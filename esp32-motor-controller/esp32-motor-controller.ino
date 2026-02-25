/*
 * ESP32 Brushless Motor Controller with Web Interface
 *
 * Wiring:
 *   ESP32 GPIO 13  -->  ESC Signal wire (white/yellow)
 *   ESP32 GND      -->  ESC Ground wire (black/brown)
 *   Battery        -->  ESC Power wires (thick red/black)
 *   Motor 3 wires  -->  ESC 3-phase output (any order, swap 2 to reverse)
 *
 * The ESC expects a PWM signal identical to a servo:
 *   - 1000us = 0% throttle (motor stopped)
 *   - 2000us = 100% throttle (full speed)
 *   - Period: 20ms (50Hz)
 */

#include <WiFi.h>
#include <WebServer.h>
#include "web_page.h"

// ─── Configuration ───────────────────────────────────────────
const char* WIFI_SSID     = "YOUR_WIFI_SSID";
const char* WIFI_PASSWORD = "YOUR_WIFI_PASSWORD";

// Set to true to create an Access Point instead of joining a network
const bool AP_MODE = true;
const char* AP_SSID     = "ESP32-Motor";
const char* AP_PASSWORD = "motor1234";

// ESC PWM configuration
const int ESC_PIN        = 13;
const int PWM_CHANNEL    = 0;
const int PWM_FREQ       = 50;       // 50Hz for ESC
const int PWM_RESOLUTION = 16;       // 16-bit: 0-65535

// Pulse width range in microseconds
const int PULSE_MIN_US = 1000;  // 0% throttle
const int PULSE_MAX_US = 2000;  // 100% throttle

// ─── Globals ─────────────────────────────────────────────────
WebServer server(80);
int currentSpeed = 0;  // 0-100%

// ─── Helper: convert percentage to duty cycle ────────────────
uint32_t speedToDuty(int percent) {
  percent = constrain(percent, 0, 100);
  int pulseUs = map(percent, 0, 100, PULSE_MIN_US, PULSE_MAX_US);
  uint32_t maxDuty = (1 << PWM_RESOLUTION) - 1;
  uint32_t duty = (uint32_t)((float)pulseUs / 20000.0f * maxDuty);
  return duty;
}

// ─── Request handlers ────────────────────────────────────────
void handleRoot() {
  server.send_P(200, "text/html", index_html);
}

void handleSpeed() {
  if (server.hasArg("value")) {
    int speed = server.arg("value").toInt();
    speed = constrain(speed, 0, 100);
    currentSpeed = speed;
    ledcWrite(ESC_PIN, speedToDuty(speed));
    Serial.printf("Speed set to: %d%%\n", speed);
    server.send(200, "text/plain", "OK");
  } else {
    server.send(400, "text/plain", "Missing value param");
  }
}

void handleStatus() {
  server.send(200, "text/plain", String(currentSpeed));
}

// ─── Setup ───────────────────────────────────────────────────
void setup() {
  Serial.begin(115200);
  Serial.println("\n=== ESP32 BLDC Motor Controller ===");

  // Configure PWM for ESC
  ledcAttachChannel(ESC_PIN, PWM_FREQ, PWM_RESOLUTION, PWM_CHANNEL);

  // Start at 0% throttle (arm the ESC)
  ledcWrite(ESC_PIN, speedToDuty(0));
  Serial.println("ESC armed at 0% throttle");
  Serial.println("Wait 2 seconds for ESC to initialize...");
  delay(2000);

  // Connect WiFi
  if (AP_MODE) {
    WiFi.softAP(AP_SSID, AP_PASSWORD);
    Serial.print("Access Point started: ");
    Serial.println(AP_SSID);
    Serial.print("IP: ");
    Serial.println(WiFi.softAPIP());
  } else {
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting to WiFi");
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 40) {
      delay(500);
      Serial.print(".");
      attempts++;
    }
    if (WiFi.status() == WL_CONNECTED) {
      Serial.print("\nConnected! IP: ");
      Serial.println(WiFi.localIP());
    } else {
      Serial.println("\nWiFi failed! Starting AP as fallback...");
      WiFi.softAP(AP_SSID, AP_PASSWORD);
      Serial.print("Fallback AP IP: ");
      Serial.println(WiFi.softAPIP());
    }
  }

  // Register routes
  server.on("/", handleRoot);
  server.on("/speed", handleSpeed);
  server.on("/status", handleStatus);

  server.begin();
  Serial.println("Web server started on port 80");
  Serial.println("Open browser and navigate to the IP above");
}

// ─── Loop ────────────────────────────────────────────────────
void loop() {
  server.handleClient();
}
