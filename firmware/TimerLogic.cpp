#include "TimerLogic.h"

TimerLogic::TimerLogic()
    : timerStartMillis(0), lastTimerUpdateMillis(0) {}

void TimerLogic::startTimerUp(unsigned long minutes) {
  snapshot.mode = TIMER_UP;
  snapshot.active = true;
  snapshot.currentSeconds = 0;
  snapshot.durationSeconds = minutes * 60UL;
  timerStartMillis = millis();
  lastTimerUpdateMillis = millis();
}

void TimerLogic::startTimerDown(unsigned long minutes) {
  snapshot.mode = TIMER_DOWN;
  snapshot.active = true;
  snapshot.durationSeconds = minutes * 60UL;
  snapshot.currentSeconds = snapshot.durationSeconds;
  timerStartMillis = millis();
  lastTimerUpdateMillis = millis();
}

void TimerLogic::stopTimer() {
  snapshot.mode = TIMER_OFF;
  snapshot.active = false;
  snapshot.currentSeconds = 0;
  snapshot.durationSeconds = 0;
}

bool TimerLogic::update(bool animationActive) {
  if (!snapshot.active || snapshot.mode == TIMER_OFF) {
    return false;
  }

  if (animationActive) {
    return false;
  }

  unsigned long now = millis();
  if (now - lastTimerUpdateMillis < 250) {
    return false;
  }

  unsigned long elapsedSec = (now - timerStartMillis) / 1000;
  unsigned long previousSeconds = snapshot.currentSeconds;

  if (snapshot.mode == TIMER_UP) {
    syncCountUp(elapsedSec);
  } else if (snapshot.mode == TIMER_DOWN) {
    syncCountdown(elapsedSec);
  }

  lastTimerUpdateMillis = now;

  return previousSeconds != snapshot.currentSeconds;
}

void TimerLogic::syncCountUp(unsigned long elapsedSec) {
  snapshot.currentSeconds = elapsedSec;
}

void TimerLogic::syncCountdown(unsigned long elapsedSec) {
  if (elapsedSec >= snapshot.durationSeconds) {
    snapshot.currentSeconds = 0;
    snapshot.active = false;
    snapshot.mode = TIMER_OFF;
    return;
  }

  snapshot.currentSeconds = snapshot.durationSeconds - elapsedSec;
}


