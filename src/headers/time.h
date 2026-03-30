#pragma once
#include <Arduino.h>

namespace Time {
  void init();
  void update();
  int getHour();
  int getMinute();
  bool isNightMode();
  String getFormattedTime();
  void setNightModeManual(bool value);
  void clearNightModeManual();
  bool isNightModeManualActive();
}