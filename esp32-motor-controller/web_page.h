#ifndef WEB_PAGE_H
#define WEB_PAGE_H

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>BLDC Motor Control</title>
  <style>
    * { box-sizing: border-box; margin: 0; padding: 0; }
    body {
      font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', sans-serif;
      background: #0f172a;
      color: #e2e8f0;
      min-height: 100vh;
      display: flex;
      flex-direction: column;
      align-items: center;
      justify-content: center;
      padding: 20px;
    }
    h1 { font-size: 1.5rem; margin-bottom: 8px; color: #38bdf8; }
    .status { font-size: 0.85rem; color: #94a3b8; margin-bottom: 30px; }
    .card {
      background: #1e293b;
      border-radius: 16px;
      padding: 30px;
      width: 100%;
      max-width: 400px;
      box-shadow: 0 4px 24px rgba(0,0,0,0.3);
    }
    .speed-display {
      text-align: center;
      font-size: 3rem;
      font-weight: 700;
      color: #38bdf8;
      margin-bottom: 4px;
    }
    .speed-label {
      text-align: center;
      font-size: 0.85rem;
      color: #64748b;
      margin-bottom: 24px;
    }
    input[type="range"] {
      -webkit-appearance: none;
      width: 100%;
      height: 12px;
      border-radius: 6px;
      background: #334155;
      outline: none;
      margin-bottom: 24px;
    }
    input[type="range"]::-webkit-slider-thumb {
      -webkit-appearance: none;
      width: 28px;
      height: 28px;
      border-radius: 50%;
      background: #38bdf8;
      cursor: pointer;
      box-shadow: 0 2px 8px rgba(56,189,248,0.4);
    }
    .buttons {
      display: flex;
      gap: 10px;
    }
    button {
      flex: 1;
      padding: 12px;
      border: none;
      border-radius: 10px;
      font-size: 1rem;
      font-weight: 600;
      cursor: pointer;
      transition: transform 0.1s, opacity 0.2s;
    }
    button:active { transform: scale(0.96); }
    .btn-stop { background: #ef4444; color: white; }
    .btn-slow { background: #f59e0b; color: #1e293b; }
    .btn-mid { background: #38bdf8; color: #1e293b; }
    .btn-max { background: #22c55e; color: #1e293b; }
    .armed-indicator {
      text-align: center;
      margin-top: 20px;
      padding: 8px 16px;
      border-radius: 8px;
      font-size: 0.85rem;
      font-weight: 600;
    }
    .armed { background: #22c55e22; color: #22c55e; }
    .disarmed { background: #ef444422; color: #ef4444; }
  </style>
</head>
<body>
  <h1>BLDC Motor Controller</h1>
  <p class="status">ESP32 | WiFi Connected</p>
  <div class="card">
    <div class="speed-display" id="speedVal">0%</div>
    <div class="speed-label">MOTOR SPEED</div>
    <input type="range" id="slider" min="0" max="100" value="0">
    <div class="buttons">
      <button class="btn-stop" onclick="setSpeed(0)">STOP</button>
      <button class="btn-slow" onclick="setSpeed(25)">25%</button>
      <button class="btn-mid" onclick="setSpeed(50)">50%</button>
      <button class="btn-max" onclick="setSpeed(100)">MAX</button>
    </div>
    <div class="armed-indicator disarmed" id="armStatus">DISARMED - Slide to arm</div>
  </div>

  <script>
    const slider = document.getElementById('slider');
    const speedVal = document.getElementById('speedVal');
    const armStatus = document.getElementById('armStatus');
    let debounceTimer;

    function setSpeed(val) {
      slider.value = val;
      speedVal.textContent = val + '%';
      updateArmStatus(val);
      sendSpeed(val);
    }

    function updateArmStatus(val) {
      if (val > 0) {
        armStatus.textContent = 'ARMED - Motor running at ' + val + '%';
        armStatus.className = 'armed-indicator armed';
      } else {
        armStatus.textContent = 'DISARMED - Motor stopped';
        armStatus.className = 'armed-indicator disarmed';
      }
    }

    function sendSpeed(val) {
      fetch('/speed?value=' + val).catch(function(){});
    }

    slider.addEventListener('input', function() {
      var val = this.value;
      speedVal.textContent = val + '%';
      updateArmStatus(val);
      clearTimeout(debounceTimer);
      debounceTimer = setTimeout(function() { sendSpeed(val); }, 50);
    });
  </script>
</body>
</html>
)rawliteral";

#endif
