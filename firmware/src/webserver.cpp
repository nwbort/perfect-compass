#include "webserver.h"
#include "pins.h"
#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>

static ::WebServer server(80);

// Embedded HTML - served from progmem to save RAM
static const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>Perfect Compass</title>
<style>
*{box-sizing:border-box;margin:0;padding:0}
body{font-family:-apple-system,BlinkMacSystemFont,'Segoe UI',sans-serif;background:#1a1a2e;color:#eee;min-height:100vh;display:flex;justify-content:center;align-items:center}
.container{background:#16213e;border-radius:16px;padding:2rem;max-width:420px;width:90%;box-shadow:0 8px 32px rgba(0,0,0,.4)}
h1{text-align:center;margin-bottom:1.5rem;color:#e94560;font-size:1.4rem}
.status{background:#0f3460;border-radius:8px;padding:1rem;margin-bottom:1.5rem;font-size:.85rem;line-height:1.6}
.status span{color:#e94560;font-weight:600}
label{display:block;margin-bottom:.3rem;font-size:.9rem;color:#aaa}
input,select{width:100%;padding:.6rem;border:1px solid #333;border-radius:8px;background:#0f3460;color:#eee;font-size:1rem;margin-bottom:1rem}
input:focus{outline:none;border-color:#e94560}
.row{display:flex;gap:.8rem}
.row>div{flex:1}
button{width:100%;padding:.75rem;border:none;border-radius:8px;font-size:1rem;font-weight:600;cursor:pointer;margin-bottom:.5rem;transition:background .2s}
.btn-primary{background:#e94560;color:#fff}
.btn-primary:hover{background:#c73652}
.btn-secondary{background:#533483;color:#fff}
.btn-secondary:hover{background:#3f2766}
.btn-warn{background:#e97d00;color:#fff}
.btn-warn:hover{background:#c56a00}
.msg{text-align:center;padding:.5rem;border-radius:8px;margin-top:.5rem;display:none;font-size:.9rem}
.msg.ok{display:block;background:#1b4332;color:#95d5b2}
.msg.err{display:block;background:#4a1010;color:#f5a5a5}
hr{border:none;border-top:1px solid #333;margin:1rem 0}
</style>
</head>
<body>
<div class="container">
<h1>&#127993; Perfect Compass</h1>

<div class="status" id="status">
  Loading status...
</div>

<label for="name">Target Name</label>
<input type="text" id="name" placeholder="e.g. Home, Office, Secret Spot" maxlength="31">

<div class="row">
  <div>
    <label for="lat">Latitude</label>
    <input type="number" id="lat" step="any" placeholder="40.7128">
  </div>
  <div>
    <label for="lon">Longitude</label>
    <input type="number" id="lon" step="any" placeholder="-74.0060">
  </div>
</div>

<button class="btn-primary" onclick="saveTarget()">Save Target</button>

<hr>

<label for="decl">Magnetic Declination (&deg;)</label>
<input type="number" id="decl" step="0.1" placeholder="0.0">
<button class="btn-secondary" onclick="saveDecl()">Update Declination</button>

<hr>

<button class="btn-warn" onclick="startCal()">Start Compass Calibration</button>
<button class="btn-secondary" onclick="stopCal()">Stop Calibration</button>

<div class="msg" id="msg"></div>
</div>

<script>
function showMsg(text, ok) {
  const m = document.getElementById('msg');
  m.textContent = text;
  m.className = 'msg ' + (ok ? 'ok' : 'err');
  setTimeout(() => m.className = 'msg', 3000);
}

async function loadStatus() {
  try {
    const r = await fetch('/api/status');
    const d = await r.json();
    document.getElementById('status').innerHTML =
      '<span>GPS:</span> ' + (d.gps_valid ? d.lat.toFixed(6) + ', ' + d.lon.toFixed(6) : 'No fix') +
      ' (' + d.satellites + ' sat)<br>' +
      '<span>Heading:</span> ' + d.heading.toFixed(1) + '&deg;<br>' +
      '<span>Target:</span> ' + (d.has_target ? d.target_name + ' (' + d.target_lat.toFixed(6) + ', ' + d.target_lon.toFixed(6) + ')' : 'Not set') +
      (d.has_target && d.gps_valid ? '<br><span>Distance:</span> ' + d.distance + ' | <span>Bearing:</span> ' + d.bearing.toFixed(1) + '&deg;' : '');

    if (d.has_target) {
      document.getElementById('name').placeholder = d.target_name;
      document.getElementById('lat').placeholder = d.target_lat;
      document.getElementById('lon').placeholder = d.target_lon;
    }
    document.getElementById('decl').value = d.declination;
  } catch(e) {
    document.getElementById('status').innerHTML = 'Could not load status';
  }
}

async function saveTarget() {
  const name = document.getElementById('name').value || 'Target';
  const lat = parseFloat(document.getElementById('lat').value);
  const lon = parseFloat(document.getElementById('lon').value);
  if (isNaN(lat) || isNaN(lon)) { showMsg('Enter valid coordinates', false); return; }
  if (lat < -90 || lat > 90 || lon < -180 || lon > 180) { showMsg('Coordinates out of range', false); return; }
  try {
    const r = await fetch('/api/target', {
      method: 'POST',
      headers: {'Content-Type': 'application/json'},
      body: JSON.stringify({name, lat, lon})
    });
    const d = await r.json();
    showMsg(d.ok ? 'Target saved!' : 'Error saving target', d.ok);
    loadStatus();
  } catch(e) { showMsg('Connection error', false); }
}

async function saveDecl() {
  const decl = parseFloat(document.getElementById('decl').value);
  if (isNaN(decl)) { showMsg('Enter valid declination', false); return; }
  try {
    const r = await fetch('/api/declination', {
      method: 'POST',
      headers: {'Content-Type': 'application/json'},
      body: JSON.stringify({declination: decl})
    });
    const d = await r.json();
    showMsg(d.ok ? 'Declination updated!' : 'Error', d.ok);
  } catch(e) { showMsg('Connection error', false); }
}

async function startCal() {
  try {
    await fetch('/api/calibrate/start', {method:'POST'});
    showMsg('Calibration started - rotate the device slowly', true);
  } catch(e) { showMsg('Connection error', false); }
}

async function stopCal() {
  try {
    await fetch('/api/calibrate/stop', {method:'POST'});
    showMsg('Calibration saved!', true);
  } catch(e) { showMsg('Connection error', false); }
}

loadStatus();
setInterval(loadStatus, 2000);
</script>
</body>
</html>
)rawliteral";

// Pointers stored by begin()
static Config* sConfig = nullptr;
static CompassModule* sCompass = nullptr;
static GpsModule* sGps = nullptr;

static void handleRoot() {
    server.send(200, "text/html", INDEX_HTML);
}

static void handleStatus() {
    StaticJsonDocument<512> doc;
    GpsData gps = sGps->getData();
    TargetLocation tgt = sConfig->getTarget();

    doc["gps_valid"] = gps.valid;
    doc["lat"] = gps.latitude;
    doc["lon"] = gps.longitude;
    doc["satellites"] = gps.satellites;
    doc["heading"] = sCompass->getHeading();
    doc["has_target"] = sConfig->hasTarget();
    doc["target_name"] = tgt.name;
    doc["target_lat"] = tgt.latitude;
    doc["target_lon"] = tgt.longitude;
    doc["declination"] = sConfig->getDeclination();
    doc["calibrating"] = sCompass->isCalibrating();

    if (sConfig->hasTarget() && gps.valid) {
        float bearing = Navigation::bearingTo(gps.latitude, gps.longitude, tgt.latitude, tgt.longitude);
        double dist = Navigation::distanceTo(gps.latitude, gps.longitude, tgt.latitude, tgt.longitude);
        doc["bearing"] = bearing;
        char distBuf[16];
        Navigation::formatDistance(dist, distBuf, sizeof(distBuf));
        doc["distance"] = distBuf;
    }

    String output;
    serializeJson(doc, output);
    server.send(200, "application/json", output);
}

static void handleSetTarget() {
    if (!server.hasArg("plain")) {
        server.send(400, "application/json", "{\"ok\":false,\"error\":\"no body\"}");
        return;
    }

    StaticJsonDocument<256> doc;
    DeserializationError err = deserializeJson(doc, server.arg("plain"));
    if (err) {
        server.send(400, "application/json", "{\"ok\":false,\"error\":\"invalid json\"}");
        return;
    }

    double lat = doc["lat"] | 0.0;
    double lon = doc["lon"] | 0.0;
    const char* name = doc["name"] | "Target";

    if (lat < -90 || lat > 90 || lon < -180 || lon > 180) {
        server.send(400, "application/json", "{\"ok\":false,\"error\":\"coordinates out of range\"}");
        return;
    }

    sConfig->setTarget(lat, lon, name);
    server.send(200, "application/json", "{\"ok\":true}");
}

static void handleSetDeclination() {
    if (!server.hasArg("plain")) {
        server.send(400, "application/json", "{\"ok\":false}");
        return;
    }

    StaticJsonDocument<64> doc;
    deserializeJson(doc, server.arg("plain"));
    float decl = doc["declination"] | 0.0f;

    sConfig->setDeclination(decl);
    sCompass->setDeclination(decl);
    server.send(200, "application/json", "{\"ok\":true}");
}

static void handleCalibrateStart() {
    sCompass->startCalibration();
    server.send(200, "application/json", "{\"ok\":true}");
}

static void handleCalibrateStop() {
    sCompass->endCalibration();
    server.send(200, "application/json", "{\"ok\":true}");
}

void WebServer::begin(Config* config, CompassModule* compass, GpsModule* gps) {
    _config = config;
    _compass = compass;
    _gps = gps;

    // Store in static vars for handlers
    sConfig = config;
    sCompass = compass;
    sGps = gps;

    // Start WiFi AP
    WiFi.mode(WIFI_AP);
    WiFi.softAP(config->getWiFiSSID(), config->getWiFiPassword());
    delay(100);

    Serial.print("[Web] AP started. IP: ");
    Serial.println(WiFi.softAPIP());

    // Register routes
    server.on("/", HTTP_GET, handleRoot);
    server.on("/api/status", HTTP_GET, handleStatus);
    server.on("/api/target", HTTP_POST, handleSetTarget);
    server.on("/api/declination", HTTP_POST, handleSetDeclination);
    server.on("/api/calibrate/start", HTTP_POST, handleCalibrateStart);
    server.on("/api/calibrate/stop", HTTP_POST, handleCalibrateStop);

    server.begin();
    Serial.println("[Web] HTTP server started on port 80");
}

void WebServer::handleClient() {
    server.handleClient();
}
