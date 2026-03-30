#include "../headers/chodec.h"                    // Knižnice
#include "../headers/semafor.h"
#include "../headers/config.h"    
#include <Arduino.h>                              

namespace Chodec                   
{
  void init()                                     // Inicializácia
  {
    pinMode(PIN_CHODEC_RED,   OUTPUT);
    pinMode(PIN_CHODEC_GREEN, OUTPUT);

    digitalWrite(PIN_CHODEC_RED,   LOW);
    digitalWrite(PIN_CHODEC_GREEN, LOW);

    Serial.println("Chodec je pripraveny");
  }
  void update()                                   // Update
  {
    switch (Semafor::getState()) {
      case Semafor::RED:
        digitalWrite(PIN_CHODEC_RED,   LOW);
        digitalWrite(PIN_CHODEC_GREEN, HIGH);
        break;
      case Semafor::GREEN:
        digitalWrite(PIN_CHODEC_RED,   HIGH);
        digitalWrite(PIN_CHODEC_GREEN, LOW);
        break;
      case Semafor::ORANGE:
        digitalWrite(PIN_CHODEC_RED,   HIGH);
        digitalWrite(PIN_CHODEC_GREEN, LOW);
        break;

      case Semafor::NIGHT:
        digitalWrite(PIN_CHODEC_RED,   LOW);
        digitalWrite(PIN_CHODEC_GREEN, LOW);
        break;
    }
  }
}
