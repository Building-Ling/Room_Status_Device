#ifndef TIMER_TYPES_H
#define TIMER_TYPES_H

#include <Arduino.h>

enum TimerMode {
  TIMER_OFF,
  TIMER_UP,
  TIMER_DOWN
};

struct TimerSnapshot {
  TimerMode mode;
  bool active;
  unsigned long currentSeconds;
  unsigned long durationSeconds;

  TimerSnapshot()
    : mode(TIMER_OFF),
      active(false),
      currentSeconds(0),
      durationSeconds(0) {}
};

#endif


