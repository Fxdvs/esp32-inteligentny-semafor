#pragma once

namespace Semafor {         
  enum State {
    RED,
    ORANGE,
    GREEN,
    NIGHT
  };

  void init();
  void update();
  State getState();
  unsigned long getTimeRemaining();
  void setNightMode(bool enabled);
  void requestChodec();
}