#ifndef TIMER_LOGIC_H
#define TIMER_LOGIC_H

#include <Arduino.h>

#include "TimerTypes.h"

class TimerLogic {
 public:
  TimerLogic();

  void startTimerUp(unsigned long minutes);
  void startTimerDown(unsigned long minutes);
  void stopTimer();

  bool update(bool animationActive);

  const TimerSnapshot &getSnapshot() const { return snapshot; }

 private:
  TimerSnapshot snapshot;
  unsigned long timerStartMillis;
  unsigned long lastTimerUpdateMillis;

  void syncCountdown(unsigned long elapsedSec);
  void syncCountUp(unsigned long elapsedSec);
};

#endif


