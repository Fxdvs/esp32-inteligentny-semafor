#include "../headers/wifi.h"                      // Knižnice
#include "../headers/config.h"
#include <Arduino.h>
#include <WiFi.h>

namespace Wifi {
  unsigned long lastReconnectAttempt = 0;         // Čas posledného pokusu o reconnect
  int reconnectAttempts = 0;                      // Počet pokusov o reconnect
  static const int MAX_RECONNECT_ATTEMPTS = 3;

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
  void update()                                   // Reconnect ak WiFi vypadlo
  {
    if (WiFi.status() == WL_CONNECTED) {
      reconnectAttempts = 0;                      // Reset počítadla po úspešnom pripojení
      return;
    }

    if (reconnectAttempts >= MAX_RECONNECT_ATTEMPTS) return;
    if (millis() - lastReconnectAttempt < WIFI_TIMEOUT) return;

    reconnectAttempts++;
    lastReconnectAttempt = millis();

    Serial.print("WiFi: pokus o reconnect ");
    Serial.print(reconnectAttempts);
    Serial.print("/");
    Serial.println(MAX_RECONNECT_ATTEMPTS);

    WiFi.disconnect();
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    if (reconnectAttempts >= MAX_RECONNECT_ATTEMPTS) {
      Serial.println("WiFi: nepripojena, idem bez siete.");
    }
  }
  bool isConnected()                              // Zistenie pripojenia
  {
    return WiFi.status() == WL_CONNECTED;
  }
}