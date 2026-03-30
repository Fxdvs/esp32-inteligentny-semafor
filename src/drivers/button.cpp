#include "../headers/button.h"                    // Knižnice
#include "../headers/config.h"
#include <Arduino.h>                              

namespace Button
{
  bool pressed = false;                           // Stav tlacitka (true = práve stlačené v tejto iterácii)
  bool lastPinState = HIGH;                       // Predchádzajúci stav pinu (INPUT_PULLUP = HIGH v kľude)
  unsigned long lastDebounce = 0;                 // Posledný debouncing

  void init()                                     // Inicializácia
  {
    pinMode(PIN_BUTTON, INPUT_PULLUP);
    Serial.println("Button je pripraveny");
  }
  void update()                                   // Update
  {
    pressed = false;                              // Vynuluj každý cyklus — true len pri hrane

    bool currentPin = digitalRead(PIN_BUTTON);

    // Detekcia zostupnej hrany (HIGH -> LOW = stlačenie)
    if (lastPinState == HIGH && currentPin == LOW) {
      if (millis() - lastDebounce >= 200) {
        pressed = true;
        lastDebounce = millis();
        Serial.println("Tlacidlo: stlacene");
      }
    }

    lastPinState = currentPin;
  }
  bool isPressed()                                // Stav stlacenia
  {
    return pressed;
  }
}
