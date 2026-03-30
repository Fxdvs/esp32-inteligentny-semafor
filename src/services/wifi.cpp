#include "../headers/wifi.h"                      // Knižnice
#include "../headers/config.h"
#include <Arduino.h>
#include <WiFi.h>

namespace Wifi {
  void init()                                     // Inicializácia
  {
    Serial.print("Pripajam sa na WiFi: ");
    Serial.println(WIFI_SSID);

    // Pripojenie na WiFi
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);         

    unsigned long start = millis();
    while (WiFi.status() != WL_CONNECTED) {
      if (millis() - start > WIFI_TIMEOUT) {
        Serial.println("Pripojenie zlyhalo, pokracujem bez WiFi.");
        return;
      }
      delay(500);
      Serial.print(".");
    }

    Serial.println("");
    Serial.print("WiFi pripojene! IP adresa: ");
    Serial.println(WiFi.localIP());
  }
  bool isConnected()                              // Zistenie pripojenia
  {
    return WiFi.status() == WL_CONNECTED;
  }
}