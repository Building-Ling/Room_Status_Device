#ifndef TIMER_LOGIC_H
#define TIMER_LOGIC_H

#include <Arduino.h>

#include "TimerTypes.h"

class TimerLogic {
 public:
  TimerLogic();

  void startTimerUp(unsigned long totalSeconds);
  void startTimerDown(unsigned long totalSeconds);

  void pause();
  void clear();
  void reset();

  // Legacy API retained for backwards compatibility; now behaves like pause().
  void stopTimer() { pause(); }

  bool update(bool animationActive);

  const TimerSnapshot &getSnapshot() const { return snapshot; }

 private:
  TimerSnapshot snapshot;
  unsigned long timerStartMillis;
  unsigned long lastTimerUpdateMillis;

  bool paused;

  static bool parseFlexibleTime(const String &input, unsigned long &outSeconds);

  void syncCountdown(unsigned long elapsedSec);
  void syncCountUp(unsigned long elapsedSec);
};

#endif


