#include "../headers/web.h"
#include "../headers/wifi.h"
#include "../headers/time.h"
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
      String html = R"(
        <!DOCTYPE html>
        <html lang='sk'>
        <head>
          <meta charset='UTF-8'>
          <meta name='viewport' content='width=device-width, initial-scale=1'>
          <title>Inteligentny semafor</title>
          <style>
            body { font-family: monospace; background: #1a1a2e; color: #eee; padding: 20px; max-width: 500px; margin: auto; }
            h1 { color: #00d4ff; }
            .card { background: #16213e; border-radius: 12px; padding: 16px; margin-bottom: 16px; }
            .label { font-size: 12px; color: #aaa; }
            .value { font-size: 20px; font-weight: bold; }
          </style>
        </head>
        <body>
          <h1>Inteligentny semafor</h1>
          <div class='card'>
            <div class='label'>Aktualny cas</div>
            <div class='value'>)" + Time::getFormattedTime() + R"(</div>
          </div>
          <div class='card'>
            <div class='label'>Nocny rezim</div>
            <div class='value'>)" + (Time::isNightMode() ? "Zapnuty" : "Vypnuty") + R"(</div>
          </div>
          <meta http-equiv='refresh' content='5'>
        </body>
        </html>
      )";
      request->send(200, "text/html", html);
    });

    // JSON endpoint pre stav
    server.on("/status", HTTP_GET, [](AsyncWebServerRequest* request) {
      String json = "{";
      json += "\"cas\":\"" + Time::getFormattedTime() + "\",";
      json += "\"noc\":" + String(Time::isNightMode() ? "true" : "false");
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