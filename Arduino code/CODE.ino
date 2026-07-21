#include <ESP32Servo.h>
#include <WiFi.h>
#include <WebServer.h>

// ============================================================
// 1. NETWORK CONFIGURATION
// ============================================================
const char* ssid     = "USERNAME";      
const char* password = "PASSWORD";  

WebServer server(80);

// ============================================================
// 2. HARDWARE PIN DEFINITIONS
// ============================================================
#define IN1 14        // Stepper Motor Coil 1
#define IN2 27        // Stepper Motor Coil 2
#define IN3 26        // Stepper Motor Coil 3
#define IN4 25        // Stepper Motor Coil 4
#define ir      33    // IR Sensor for Entry Detection
#define proxi   32    // Inductive Proximity Sensor (Metal)
#define buzzer  12    // Feedback Buzzer
#define rainPin 34    // Moisture/Rain Sensor

// ============================================================
// 3. GLOBAL VARIABLES & CONSTANTS
// ============================================================
Servo servo1;
int servoAng = 95;    // Initial Servo Angle
int step120 = 680;    // Steps for 120 degree rotation
int step240 = 1360;   // Steps for 240 degree rotation

const int WASTE_LIMIT = 5; // Max capacity per bin
int wetCount = 0, dryCount = 0, metalCount = 0;
String lastWaste = "None";
bool lastButtonState = HIGH, lastIRState = HIGH;

// Stepper Motor Half-Step Sequence
int steps[4][4] = {{1,0,0,1}, {1,0,1,0}, {0,1,1,0}, {0,1,0,1}};
int currentStep = 0;

// ============================================================
// 4. MOTOR CONTROL FUNCTIONS
// ============================================================
void stepperMove(int stepsToMove) {
  int dir = (stepsToMove > 0) ? 1 : -1;
  stepsToMove = abs(stepsToMove);
  for (int i = 0; i < stepsToMove; i++) {
    currentStep += dir;
    if (currentStep > 3) currentStep = 0;
    if (currentStep < 0) currentStep = 3;
    digitalWrite(IN1, steps[currentStep][0]);
    digitalWrite(IN2, steps[currentStep][1]);
    digitalWrite(IN3, steps[currentStep][2]);
    digitalWrite(IN4, steps[currentStep][3]);
    delay(5);
    server.handleClient(); // Keep web server responsive during movement
  }
}

void stopMotor() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
}

void dropWaste() {
  servo1.write(180); // Open flap
  delay(1500);
  servo1.write(servoAng); // Close flap
  delay(500);
}

// ============================================================
// 5. WEB SERVER API HANDLERS (JSON & RESET)
// ============================================================
void handleData() {
  int total = wetCount + dryCount + metalCount;
  float wP = (total > 0) ? (wetCount * 100.0 / total) : 0;
  float dP = (total > 0) ? (dryCount * 100.0 / total) : 0;
  float mP = (total > 0) ? (metalCount * 100.0 / total) : 0;

  String json = "{";
  json += "\"total\":" + String(total) + ",";
  json += "\"wet\":" + String(wetCount) + ",";
  json += "\"dry\":" + String(dryCount) + ",";
  json += "\"metal\":" + String(metalCount) + ",";
  json += "\"wetPct\":" + String(wP,1) + ",";
  json += "\"dryPct\":" + String(dP,1) + ",";
  json += "\"metalPct\":" + String(mP,1) + ",";
  json += "\"limit\":" + String(WASTE_LIMIT) + ",";
  json += "\"last\":\"" + lastWaste + "\"";
  json += "}";
  server.send(200, "application/json", json);
}

void handleReset() {
  wetCount = 0; dryCount = 0; metalCount = 0; lastWaste = "None";
  server.send(200, "application/json", "{\"status\":\"reset\"}");
}

// ============================================================
// 6. DASHBOARD INTERFACE (HTML, CSS & JAVASCRIPT)
// ============================================================
void handleRoot() {
  String html = R"rawhtml(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8"/>
<meta name="viewport" content="width=device-width, initial-scale=1.0"/>
<title>Smart Waste Segregation</title>
<link href="https://fonts.googleapis.com/css2?family=Poppins:wght@400;600;700&family=Roboto+Mono:wght@400;700&display=swap" rel="stylesheet">
<script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
<style>
  :root {
    --bg: #0f1117; --surface: #181c27; --border: rgba(255,255,255,0.07);
    --text: #e8eaf0; --muted: #6b7280; --green-light: #4caf50;
    --orange-light: #f5a623; --red-light: #ef5350; --accent: #7c6af7; --radius: 16px;
  }
  * { box-sizing: border-box; margin: 0; padding: 0; }
  body {
    font-family: 'Poppins', sans-serif; background: var(--bg); color: var(--text);
    min-height: 100vh; padding: 24px 16px 48px;
    background-image: radial-gradient(ellipse 60% 40% at 20% 10%, rgba(124,106,247,0.08) 0%, transparent 60%);
  }
  .container { max-width: 900px; margin: 0 auto; }
  .card { background: var(--surface); border: 1px solid var(--border); border-radius: var(--radius); padding: 25px; margin-bottom: 16px; position: relative; }
  .header { text-align: center; border-top: 3px solid var(--accent); }
  .header h1 { font-size: 1.8rem; font-weight: 800; color: #fff; }
  .total-num { font-family: 'Roboto Mono', monospace; font-size: 3.5rem; font-weight: 700; text-align: center; }
  .bins { display: grid; grid-template-columns: repeat(3,1fr); gap: 14px; margin-bottom: 16px; }
  @media(max-width:560px){ .bins{ grid-template-columns:1fr; } }
  .bin-card { border-radius: var(--radius); padding: 20px; text-align: center; border: 1px solid var(--border); background: var(--surface); }
  .bin-pct { font-family: 'Roboto Mono', monospace; font-size: 2rem; font-weight: 700; }
  .bin-count { font-family: 'Roboto Mono', monospace; font-size: 0.85rem; margin-top: 5px; opacity: 0.8; font-weight: 600; }
  #graph-box { display: none; height: 350px; }
  .bottom-row { display: flex; align-items: center; justify-content: space-between; flex-wrap: wrap; gap: 14px; }
  .action-btn { font-size: 0.8rem; font-weight: 700; padding: 10px 20px; border: 1px solid var(--border); border-radius: 8px; background: transparent; color: var(--text); cursor: pointer; transition: 0.2s; }
  .action-btn:hover { background: rgba(255,255,255,0.05); }
  .log-box { background: var(--surface); border: 1px solid var(--border); border-radius: var(--radius); padding: 15px; height: 160px; overflow-y: auto; display: flex; flex-direction: column-reverse; font-family: 'Roboto Mono', monospace; font-size: 0.8rem; color: var(--muted); }

  @media print {
    body { background: white !important; color: black !important; background-image: none !important; }
    .action-btn, .header::before { display: none !important; }
    .card, .bin-card, .log-box { border: 1px solid #000 !important; background: white !important; color: black !important; box-shadow: none !important; overflow: visible !important; }
    #graph-box { display: block !important; height: auto !important; }
    .total-num, .bin-pct, h1 { color: black !important; }
  }
</style>
</head>
<body>
<div class="container">
  <div class="card header"><h1>Smart Waste Segregation</h1></div>
  <div class="card"><div style="font-size:0.7rem; color:var(--muted); text-align:center">TOTAL UNITS</div><div class="total-num" id="total">0</div></div>
  
  <div class="bins">
    <div class="bin-card">
      <div style="color:var(--green-light)">WET</div>
      <div class="bin-pct" id="wet-pct">0%</div>
      <div class="bin-count" id="wet-c" style="color:var(--green-light)">Items: 0/5</div>
    </div>
    <div class="bin-card">
      <div style="color:var(--orange-light)">DRY</div>
      <div class="bin-pct" id="dry-pct">0%</div>
      <div class="bin-count" id="dry-c" style="color:var(--orange-light)">Items: 0/5</div>
    </div>
    <div class="bin-card">
      <div style="color:var(--red-light)">METAL</div>
      <div class="bin-pct" id="metal-pct">0%</div>
      <div class="bin-count" id="metal-c" style="color:var(--red-light)">Items: 0/5</div>
    </div>
  </div>

  <div class="card" id="graph-box">
    <div style="font-size:0.7rem; color:var(--muted); margin-bottom:10px">DAILY COMPOSITION SUMMARY</div>
    <canvas id="wasteChart"></canvas>
  </div>

  <div class="card bottom-row">
    <div>Last: <span id="last-waste" style="font-weight:700">None</span></div>
    <div style="display:flex; gap:10px; flex-wrap:wrap;">
      <button class="action-btn" style="border-color:var(--accent); color:var(--accent)" onclick="toggleG()">Graph</button>
      <button class="action-btn" style="border-color:var(--green-light); color:var(--green-light)" onclick="downloadReport()">TXT Report</button>
      <button class="action-btn" style="border-color:var(--orange-light); color:var(--orange-light)" onclick="window.print()">PDF Report</button>
      <button class="action-btn" onclick="resetC()">Reset</button>
    </div>
  </div>
  <div style="margin-top:10px">
    <div style="font-size:0.7rem; color:var(--muted); margin-bottom:5px">// SYSTEM EVENT LOG</div>
    <div class="log-box" id="log-box"></div>
  </div>
</div>
<script>
  var prev = { wet:0, dry:0, metal:0 };
  var initialized = false; 

  var ctx = document.getElementById('wasteChart').getContext('2d');
  var chart = new Chart(ctx, {
    type: 'bar',
    data: { 
      // X-AXIS LABELS
      labels: ["Wet Waste", "Dry Waste", "Metal Waste"], 
      datasets: [{
        // SETTING DATASET
        backgroundColor: ['#4caf50', '#f5a623', '#ef5350'],
        data: [0, 0, 0]
      }]
    },
    options: { 
      responsive: true, 
      maintainAspectRatio: false, 
      plugins: {
        // THIS REMOVES THE "UNDEFINED" TEXT AND BLOCK
        legend: { display: false } 
      },
      scales: { 
        y: { 
          beginAtZero: true,
          max: 100, 
          title: { display: true, text: 'Waste Percentage (%)', color: '#6b7280' },
          ticks: { color: '#6b7280' },
          grid: { color: 'rgba(255,255,255,0.05)' }
        },
        x: {
          title: { display: true, text: 'Waste Type', color: '#6b7280' },
          ticks: { color: '#6b7280' },
          grid: { display: false }
        }
      }
    }
  });

  function toggleG() { 
    var b = document.getElementById('graph-box'); 
    b.style.display = (b.style.display == 'block') ? 'none' : 'block'; 
  }
  
  function addLog(m) {
    var box = document.getElementById('log-box');
    var e = document.createElement('div');
    e.innerHTML = '> [' + new Date().toLocaleTimeString().split(' ')[0] + '] ' + m;
    box.prepend(e);
  }

  function update() {
    fetch('/data').then(function(r){return r.json()}).then(function(d){
      document.getElementById('total').innerHTML = d.total;
      document.getElementById('wet-pct').innerHTML = d.wetPct + '%';
      document.getElementById('dry-pct').innerHTML = d.dryPct + '%';
      document.getElementById('metal-pct').innerHTML = d.metalPct + '%';
      document.getElementById('wet-c').innerHTML = d.wet + '/' + d.limit;
      document.getElementById('dry-c').innerHTML = d.dry + '/' + d.limit;
      document.getElementById('metal-c').innerHTML = d.metal + '/' + d.limit;
      
      // Update Graph Bars
      chart.data.datasets[0].data = [d.wetPct, d.dryPct, d.metalPct];
      chart.update();

      if(initialized){
        if(d.wet > prev.wet) addLog('Wet waste processed');
        if(d.dry > prev.dry) addLog('Dry waste processed');
        if(d.metal > prev.metal) addLog('Metal waste processed');
      } else {
        if(d.total >= 0) initialized = true; 
      }
      prev = { wet: d.wet, dry: d.dry, metal: d.metal };
    });
  }
  function resetC() { fetch('/reset').then(function(){ location.reload(); }); }
  setInterval(update, 2000);
</script>
</body>
</html>
)rawhtml";
  server.send(200, "text/html", html);
}

// ============================================================
// 7. ARDUINO SETUP (INITIALIZATION)
// ============================================================
void setup() {
  Serial.begin(115200);
  pinMode(IN1, OUTPUT); pinMode(IN2, OUTPUT); pinMode(IN3, OUTPUT); pinMode(IN4, OUTPUT);
  pinMode(proxi, INPUT); pinMode(ir, INPUT_PULLUP); pinMode(buzzer, OUTPUT);
  servo1.setPeriodHertz(50);
  servo1.attach(13, 500, 2400);
  servo1.write(servoAng);

  delay(1000); // Startup stabilization
  lastButtonState = digitalRead(proxi);
  lastIRState = digitalRead(ir);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  Serial.println("\nReady: http://" + WiFi.localIP().toString());

  // Set up Server Routes
  server.on("/", handleRoot);
  server.on("/data", handleData);
  server.on("/reset", handleReset);
  server.begin();
}

// ============================================================
// 8. MAIN PROGRAM LOOP (SENSOR DETECTION & LOGIC)
// ============================================================
void loop() {
  server.handleClient(); // Handle web requests

  int ps = digitalRead(proxi); // Read Proximity Sensor
  int is = digitalRead(ir);    // Read IR Sensor

  // --- METAL WASTE LOGIC ---
  if (ps == LOW && lastButtonState == HIGH) {
    if (metalCount < WASTE_LIMIT) {
      metalCount++; tone(buzzer, 1000, 500); lastWaste = "Metal";
      stepperMove(step240); stopMotor(); dropWaste(); stepperMove(step120); stopMotor();
    }
  }
  lastButtonState = ps;


  // --- IR SENSOR & MOISTURE LOGIC ---
  if (is == LOW && lastIRState == HIGH) {
    int sv = 0; 
    for (int i = 0; i < 3; i++) { sv += analogRead(rainPin); delay(50); } // Read 3 samples for accuracy
    float moist = map(sv / 3, 0, 4095, 0, 100);

    // --- WET WASTE DETECTION ---
    if (moist < 40) {
      if (wetCount < WASTE_LIMIT) {
        wetCount++; lastWaste = "Wet"; tone(buzzer, 1000, 300);
        stepperMove(step120); stopMotor(); dropWaste(); stepperMove(step240); stopMotor();
      }
    } 
    // --- DRY WASTE DETECTION ---
    else {
      if (dryCount < WASTE_LIMIT) {
        dryCount++; lastWaste = "Dry"; tone(buzzer, 1000, 300); dropWaste();
      }
    }
  }
  lastIRState = is;
  delay(100);
}
