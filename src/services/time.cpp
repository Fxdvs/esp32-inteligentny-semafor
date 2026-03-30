#include "../headers/time.h"                      // Knižnice
#include "../headers/wifi.h"
#include "../headers/config.h"
#include <Arduino.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

namespace Time {

  WiFiUDP ntpUDP;
  NTPClient timeClient(ntpUDP, "pool.ntp.org", UTC_OFFSET, UPDATE_INTERVAL);

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
    int hour = getHour();
    return (hour >= NIGHT_HOUR_FROM || hour < NIGHT_HOUR_TO);
  }

  String getFormattedTime()
  {
    return timeClient.getFormattedTime();
  }
}