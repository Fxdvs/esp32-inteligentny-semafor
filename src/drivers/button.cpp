#include "../headers/button.h"                    // Knižnice
#include "../headers/config.h"
#include <Arduino.h>                              

namespace Button                   
{
  bool pressed = false;                           // Stav tlacitka
  unsigned long lastDebounce = 0;                 // Posledný debouncing

  void init()                                     // Inicializácia
  {
    pinMode(PIN_BUTTON, INPUT_PULLUP);
    Serial.println("Button je pripraveny");
  }
  void update()                                   // Update
  {
    // Debounce — ignoruj rýchle zmeny pod 200ms
    if (millis() - lastDebounce < 200) return;
    if (digitalRead(PIN_BUTTON) == LOW) {
      pressed = true;
      lastDebounce = millis();
      Serial.println("Tlacidlo: stlacene");
    } else {
      pressed = false;
    }
  }
  bool isPressed()                                // Stav stlacenia
  {
    return pressed;
  }
}
