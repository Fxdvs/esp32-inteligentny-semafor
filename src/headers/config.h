#pragma once

// WiFi
#define WIFI_SSID     "laco"
#define WIFI_PASSWORD "laco123456789"
#define WIFI_TIMEOUT  10000

// Časovania semafora
#define TIME_RED         5000
#define TIME_GREEN       4000
#define TIME_GREEN_MIN   2000  // Minimálna zelená — skôr chodec nemôže skrátiť
#define TIME_ORANGE      2000
#define TIME_NIGHT_BLINK  800

// Piny semafor
#define PIN_RED    25
#define PIN_ORANGE 26
#define PIN_GREEN  27

// Piny chodec
#define PIN_CHODEC_RED   32
#define PIN_CHODEC_GREEN 33

// Piny senzor
#define SENSOR_TRIG 5
#define SENSOR_ECHO 18

// Pin tlačidlo
#define PIN_BUTTON 4

// Senzor — prah detekcie auta
#define SENSOR_CAR_THRESHOLD_CM 15.0

// Nočný režim
#define NIGHT_HOUR_FROM 22
#define NIGHT_HOUR_TO   6

// Časovanie NTP Server
#define UTC_OFFSET      7200                      // UTC+2 zima
#define UPDATE_INTERVAL 60000                     // Update každú minutu