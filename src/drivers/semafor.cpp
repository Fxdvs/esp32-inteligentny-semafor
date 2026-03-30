#include "../headers/semafor.h"                   // Knižnice
#include "../headers/sensor.h"
#include "../headers/time.h"    
#include "../headers/config.h"               
#include <Arduino.h>                              

namespace Semafor                   
{
  State currentState = RED;                       // Aktualny stav
  unsigned long stateStartTime = 0;               // Zaciatok stavu
  bool chodecRequested = false;                   // Check na požadavku od chodcu
  bool manualNightMode = false;                   // Manualny nočný režim

  void init()                                     // Inicializácia
  {
    pinMode(PIN_RED, OUTPUT);                     // Nastavenie pinov
    pinMode(PIN_ORANGE, OUTPUT);
    pinMode(PIN_GREEN, OUTPUT);
    
    currentState = RED;                           // Nastavenie stavov
    stateStartTime = millis();

    digitalWrite(PIN_RED,    HIGH);
    digitalWrite(PIN_ORANGE, LOW);
    digitalWrite(PIN_GREEN,  LOW);

    Serial.println("Semafor je pripraveny");
  }
  void setLED(bool red, bool orange, bool green)  // Zmena LED
  {
    digitalWrite(PIN_RED, red ? HIGH : LOW);
    digitalWrite(PIN_ORANGE, orange ? HIGH : LOW);
    digitalWrite(PIN_GREEN, green ? HIGH : LOW);
  };
  void changeState(State next)                    // Zmena stavu
  {
    currentState = next;
    stateStartTime = millis();

    Serial.print("Semafor -> ");
    switch (next) {
      case RED:    
        Serial.println("Cervena");  
        break;
      case ORANGE: 
        Serial.println("Oranzova"); 
        break;
      case GREEN:  
        Serial.println("Zelena");  
        break;
      case NIGHT:  
        Serial.println("Nocny rezim");      
        break;
    }
  }
  State getState()                                // Vratenie stavu 
  {
    return currentState;
  }
  void update()                                   // Update
  {
    unsigned long elapsed = millis() - stateStartTime;

    // Nočný režim — automatický podľa času alebo manuálny
    if (Time::isNightMode() || manualNightMode) {
      if (currentState != NIGHT) {
        changeState(NIGHT);
      }
    }
    switch (currentState) {
      case RED:
        setLED(true, false, false);
        if (elapsed >= TIME_RED) {
          chodecRequested = false;
          changeState(GREEN);
        }
        break;

      case GREEN:
        setLED(false, false, true);
        {
          bool carPresent  = (Sensor::getDistance() < 40.0);
          bool maxExpired  = (elapsed >= TIME_GREEN * 2);
          bool timeExpired = (elapsed >= TIME_GREEN);
          bool chodecSkrat = (chodecRequested && elapsed >= TIME_GREEN_MIN);

          if (maxExpired || (timeExpired && !carPresent) || chodecSkrat) {
            changeState(ORANGE);
          }
        }
        break;

      case ORANGE:
        setLED(false, true, false);
        if (elapsed >= TIME_ORANGE) {
          changeState(RED);
        }
        break;

      case NIGHT:
        // Blikajúca oranžová
        if (!Time::isNightMode() && !manualNightMode) {
          changeState(RED);
          break;
        }
        if (elapsed >= TIME_NIGHT_BLINK) {
          bool currentOrange = digitalRead(PIN_ORANGE);
          setLED(false, !currentOrange, false);
          stateStartTime = millis();
        }
        break;
    }
  }
  unsigned long getTimeRemaining()                // Vratenie zbyvajúceho času
  {
    unsigned long elapsed = millis() - stateStartTime;
    unsigned long total = 0;

    switch (currentState) {
      case RED:    
        total = TIME_RED;    
        break;
      case GREEN: 
        total = TIME_GREEN;  
        break;
      case ORANGE: 
        total = TIME_ORANGE; 
        break;
      default:     
        return 0;
    }

    if (elapsed >= total) return 0;
    return (total - elapsed) / 1000;
  }
  void setNightMode(bool enabled)                 // Nastavenie nočného režimu
  {
    manualNightMode = enabled;
  }
  void requestChodec()                            // Požiadavka od chodcu
  {
    chodecRequested = true;
    Serial.println("Chodec: Poziadavka zaznamenana");
  }
}

