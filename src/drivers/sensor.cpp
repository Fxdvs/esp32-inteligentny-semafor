#include "../headers/sensor.h"                    // Knižnice
#include "../headers/config.h"
#include <Arduino.h>                              

namespace Sensor                
{
  float lastDistance = 999.0;                     // Posledná vzdialenost
  unsigned long lastMeasure = 0;                  // Posledné meranie

  void init()                                     // Inicializácia
  {
    pinMode(SENSOR_TRIG, OUTPUT);
    pinMode(SENSOR_ECHO, INPUT);

    digitalWrite(SENSOR_TRIG, LOW);
    delay(100);

    Serial.println("Senzor je pripraveny");
  }
  float measure()                                 // Meranie
  {
    digitalWrite(SENSOR_TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(SENSOR_TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(SENSOR_TRIG, LOW);

    long duration = pulseIn(SENSOR_ECHO, HIGH, 25000);

    if (duration == 0) return 999.0;

    return duration * 0.034f / 2.0f;
  }
  void update()                                   // Update
  {
    // Meriame každých 250ms — nie každý loop
    if (millis() - lastMeasure < 250) return;

    lastMeasure  = millis();
    lastDistance = measure();

    if (lastDistance < SENSOR_MAX_DISTANCE_CM) {
      Serial.print("Vozidlo: ");
      Serial.print(lastDistance);
      Serial.println(" cm");
    }
  }
  float getDistance()                             // Vratenie vzdialenosti
  {
    return lastDistance;
  }

}
