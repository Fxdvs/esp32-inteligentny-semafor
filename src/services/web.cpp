#include "../headers/web.h"
#include "../headers/wifi.h"
#include "../headers/time.h"
#include "../headers/sensor.h"
#include <Arduino.h>
#include <ESPAsyncWebServer.h>

namespace Web {

  AsyncWebServer server(80);

  void init()
  {
    if (!Wifi::isConnected()) {
      Serial.println("WebServer: WiFi nie je pripojene, preskakujem");
      return;
    }

    // Hlavná stránka
    server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
      String html = R"(<!DOCTYPE html>
<html lang='sk'>
<head>
  <meta charset='UTF-8'>
  <meta name='viewport' content='width=device-width, initial-scale=1'>
  <title>Inteligentny semafor</title>
  <style>
    *, *::before, *::after { box-sizing: border-box; margin: 0; padding: 0; }
    body {
      font-family: monospace;
      background: #000;
      color: #fff;
      padding: 24px 16px;
      max-width: 480px;
      margin: auto;
      min-height: 100vh;
    }
    h1 {
      font-size: 18px;
      font-weight: bold;
      letter-spacing: 2px;
      text-transform: uppercase;
      color: #fff;
      margin-bottom: 24px;
      border-bottom: 1px solid #222;
      padding-bottom: 12px;
    }
    .card {
      background: #111;
      border: 1px solid #222;
      border-radius: 10px;
      padding: 18px 20px;
      margin-bottom: 14px;
    }
    .label {
      font-size: 11px;
      letter-spacing: 1.5px;
      text-transform: uppercase;
      color: #555;
      margin-bottom: 6px;
    }
    .value {
      font-size: 28px;
      font-weight: bold;
      color: #fff;
    }
    .value.small {
      font-size: 18px;
    }
    .badge {
      display: inline-block;
      padding: 3px 10px;
      border-radius: 4px;
      font-size: 13px;
      font-weight: bold;
      letter-spacing: 1px;
    }
    .badge-on  { background: #fff; color: #000; }
    .badge-off { background: #222; color: #777; }
    .badge-auto { background: #1a1a1a; color: #555; font-size: 11px; border: 1px solid #333; }
    .btn {
      display: block;
      width: 100%;
      padding: 14px;
      margin-top: 10px;
      background: #fff;
      color: #000;
      border: none;
      border-radius: 8px;
      font-family: monospace;
      font-size: 13px;
      font-weight: bold;
      letter-spacing: 1.5px;
      text-transform: uppercase;
      cursor: pointer;
      transition: background 0.15s, color 0.15s;
    }
    .btn:hover { background: #ddd; }
    .btn.secondary {
      background: #000;
      color: #fff;
      border: 1px solid #333;
    }
    .btn.secondary:hover { background: #111; }
    #status-msg {
      font-size: 11px;
      color: #444;
      text-align: center;
      margin-top: 8px;
      height: 16px;
    }
  </style>
</head>
<body>
  <h1>Inteligentny semafor</h1>

  <div class='card'>
    <div class='label'>Aktualny cas</div>
    <div class='value' id='cas'>--:--:--</div>
  </div>

  <div class='card'>
    <div class='label'>Vzdialenost vozidla</div>
    <div class='value' id='dist'>-- cm</div>
  </div>

  <div class='card'>
    <div class='label'>Nocny rezim</div>
    <div class='value small' id='noc-val'>--</div>
    <div style='margin-top:8px'>
      <span id='noc-source' class='badge badge-auto'>AUTO</span>
    </div>
    <button class='btn' id='btn-toggle' onclick='toggleNoc()'>Prepnut nocny rezim</button>
    <button class='btn secondary' id='btn-auto' onclick='setAuto()'>Obnovit automatiku</button>
    <div id='status-msg'></div>
  </div>

  <script>
    function showMsg(msg, color) {
      var el = document.getElementById('status-msg');
      el.style.color = color || '#444';
      el.textContent = msg;
      setTimeout(function(){ el.textContent = ''; }, 2000);
    }

    function updateUI(data) {
      document.getElementById('cas').textContent = data.cas;
      document.getElementById('dist').textContent = data.vzdialenost >= 999 ? '-- cm' : data.vzdialenost.toFixed(1) + ' cm';
      var nocEl = document.getElementById('noc-val');
      var srcEl = document.getElementById('noc-source');
      nocEl.textContent = data.noc ? 'Zapnuty' : 'Vypnuty';
      if (data.manual) {
        srcEl.textContent = 'MANUAL';
        srcEl.className = 'badge ' + (data.noc ? 'badge-on' : 'badge-off');
      } else {
        srcEl.textContent = 'AUTO';
        srcEl.className = 'badge badge-auto';
      }
    }

    function fetchStatus() {
      fetch('/status')
        .then(function(r){ return r.json(); })
        .then(updateUI)
        .catch(function(){});
    }

    function toggleNoc() {
      fetch('/toggle-night', { method: 'POST' })
        .then(function(r){ return r.json(); })
        .then(function(data){
          updateUI(data);
          showMsg('OK', '#fff');
        })
        .catch(function(){ showMsg('Chyba', '#f55'); });
    }

    function setAuto() {
      fetch('/auto-night', { method: 'POST' })
        .then(function(r){ return r.json(); })
        .then(function(data){
          updateUI(data);
          showMsg('Automatika obnovena', '#fff');
        })
        .catch(function(){ showMsg('Chyba', '#f55'); });
    }

    fetchStatus();
    setInterval(fetchStatus, 1000);
  </script>
</body>
</html>)";
      request->send(200, "text/html", html);
    });

    // JSON endpoint pre stav
    server.on("/status", HTTP_GET, [](AsyncWebServerRequest* request) {
      String json = "{";
      json += "\"cas\":\"" + Time::getFormattedTime() + "\",";
      json += "\"noc\":"    + String(Time::isNightMode() ? "true" : "false") + ",";
      json += "\"manual\":" + String(Time::isNightModeManualActive() ? "true" : "false") + ",";
      json += "\"vzdialenost\":" + String(Sensor::getDistance(), 1);
      json += "}";
      request->send(200, "application/json", json);
    });

    // Prepnutie nočného režimu (toggle)
    server.on("/toggle-night", HTTP_POST, [](AsyncWebServerRequest* request) {
      bool newVal = !Time::isNightMode();
      Time::setNightModeManual(newVal);
      String json = "{";
      json += "\"cas\":\"" + Time::getFormattedTime() + "\",";
      json += "\"noc\":"    + String(Time::isNightMode() ? "true" : "false") + ",";
      json += "\"manual\":" + String(Time::isNightModeManualActive() ? "true" : "false");
      json += "}";
      request->send(200, "application/json", json);
    });

    // Obnovenie automatiky
    server.on("/auto-night", HTTP_POST, [](AsyncWebServerRequest* request) {
      Time::clearNightModeManual();
      String json = "{";
      json += "\"cas\":\"" + Time::getFormattedTime() + "\",";
      json += "\"noc\":"    + String(Time::isNightMode() ? "true" : "false") + ",";
      json += "\"manual\":" + String(Time::isNightModeManualActive() ? "true" : "false");
      json += "}";
      request->send(200, "application/json", json);
    });

    server.begin();
    Serial.print("Web server bezi na: http://");
    Serial.println(WiFi.localIP());
  }

  void update()
  {
    // AsyncWebServer nepotrebuje update v loope
    // obsluhuje requesty automaticky
  }

}
