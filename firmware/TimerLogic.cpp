#include "TimerLogic.h"

TimerLogic::TimerLogic()
    : timerStartMillis(0), lastTimerUpdateMillis(0), paused(false) {}

void TimerLogic::startTimerUp(unsigned long totalSeconds) {
  snapshot.mode = TIMER_UP;
  snapshot.active = true;
  paused = false;

  snapshot.currentSeconds = 0;
  snapshot.durationSeconds = totalSeconds;

  unsigned long now = millis();
  timerStartMillis = now;
  lastTimerUpdateMillis = now;
}

void TimerLogic::startTimerDown(unsigned long totalSeconds) {
  snapshot.mode = TIMER_DOWN;
  snapshot.active = true;
  paused = false;

  snapshot.durationSeconds = totalSeconds;
  snapshot.currentSeconds = snapshot.durationSeconds;

  unsigned long now = millis();
  timerStartMillis = now;
  lastTimerUpdateMillis = now;
}

void TimerLogic::pause() {
  if (!snapshot.active || snapshot.mode == TIMER_OFF || paused) {
    return;
  }

  unsigned long now = millis();
  unsigned long elapsedSec = (now - timerStartMillis) / 1000;

  if (snapshot.mode == TIMER_UP) {
    syncCountUp(elapsedSec);
  } else if (snapshot.mode == TIMER_DOWN) {
    syncCountdown(elapsedSec);
  }

  paused = true;
  lastTimerUpdateMillis = now;
}

void TimerLogic::clear() {
  paused = false;

  if (snapshot.mode == TIMER_DOWN) {
    snapshot.active = false;
    snapshot.currentSeconds = snapshot.durationSeconds;
  } else if (snapshot.mode == TIMER_UP) {
    snapshot.active = false;
    snapshot.currentSeconds = 0;
  }
}

void TimerLogic::reset() {
  snapshot = TimerSnapshot();
  timerStartMillis = 0;
  lastTimerUpdateMillis = 0;
  paused = false;
}

bool TimerLogic::update(bool animationActive) {
  if (!snapshot.active || snapshot.mode == TIMER_OFF || paused) {
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

bool TimerLogic::parseFlexibleTime(const String &input,
                                   unsigned long &outSeconds) {
  String s = input;
  s.trim();
  if (s.length() == 0) {
    return false;
  }

  int firstColon = s.indexOf(':');
  int lastColon = s.lastIndexOf(':');

  long h = 0;
  long m = 0;
  long sec = 0;

  if (firstColon == -1) {
    sec = s.toInt();
  } else if (firstColon == lastColon) {
    String mm = s.substring(0, firstColon);
    String ss = s.substring(firstColon + 1);
    mm.trim();
    ss.trim();
    if (mm.length() == 0 || ss.length() == 0) return false;
    m = mm.toInt();
    sec = ss.toInt();
  } else {
    String hh = s.substring(0, firstColon);
    String mm = s.substring(firstColon + 1, lastColon);
    String ss = s.substring(lastColon + 1);
    hh.trim();
    mm.trim();
    ss.trim();
    if (hh.length() == 0 || mm.length() == 0 || ss.length() == 0) return false;
    h = hh.toInt();
    m = mm.toInt();
    sec = ss.toInt();
  }

  if (h < 0 || m < 0 || sec < 0) {
    return false;
  }

  unsigned long total =
      (unsigned long)h * 3600UL + (unsigned long)m * 60UL + (unsigned long)sec;
  outSeconds = total;
  return true;
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


