#include "../headers/time.h"                      // Knižnice
#include "../headers/wifi.h"
#include "../headers/config.h"
#include <Arduino.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

namespace Time {

  WiFiUDP ntpUDP;
  NTPClient timeClient(ntpUDP, "pool.ntp.org", UTC_OFFSET, UPDATE_INTERVAL);

  bool _manualOverrideActive = false;
  bool _manualOverrideValue  = false;

  void init()                                     // Inicializácia
  {
    if (!Wifi::isConnected()) {
      Serial.println("Cas: WiFi nie je pripojene, preskakujem NTP");
      return;
    }

    timeClient.begin();
    timeClient.update();

    Serial.print("Cas synchronizovany: ");
    Serial.println(timeClient.getFormattedTime());
  }
  void update()                                   // Update
  {
    if (Wifi::isConnected()) {
      timeClient.update();
    }
  }
  int getHour()                                   // Zistenie hodiny
  {
    return timeClient.getHours();
  }
  int getMinute()                                 // Zistenie minúty
  {
    return timeClient.getMinutes();
  }
  bool isNightMode()                              // Zistenie nočných režimov
  {
    if (_manualOverrideActive) return _manualOverrideValue;
    int hour = getHour();
    return (hour >= NIGHT_HOUR_FROM || hour < NIGHT_HOUR_TO);
  }

  void setNightModeManual(bool value)
  {
    _manualOverrideActive = true;
    _manualOverrideValue  = value;
  }

  void clearNightModeManual()
  {
    _manualOverrideActive = false;
  }

  bool isNightModeManualActive()
  {
    return _manualOverrideActive;
  }

  String getFormattedTime()
  {
    return timeClient.getFormattedTime();
  }
}