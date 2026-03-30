#include <Arduino.h>                              // Knižnice
#include "headers/semafor.h"
#include "headers/chodec.h"
#include "headers/button.h"
#include "headers/sensor.h"
#include "headers/wifi.h"
#include "headers/time.h"
#include "headers/web.h"

void setup() {                                  // Inicializácia
  Serial.begin(115200);

  // Sieť — musí byť ako prvé
  Wifi::init();
  Time::init();
  Web::init();

  // Hardware
  Sensor::init();
  Button::init();
  Semafor::init();
  Chodec::init();
}

void loop() {                                   // Loop
  // WiFi — reconnect ak vypadlo
  Wifi::update();

  // Aktualizácia času
  Time::update();

  // Tlačidlo — skontroluj pred semaforom
  Button::update();
  if (Button::isPressed()) {
    Semafor::requestChodec();
  }

  // Senzor
  Sensor::update();

  // Semafor a chodec
  Semafor::update();
  Chodec::update();
}