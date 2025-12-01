#ifndef WIRELESS_H
#define WIRELESS_H

#include <Arduino.h>

#include "DisplayManager.h"
#include "TimerLogic.h"

class WirelessHandler {
 public:
  WirelessHandler(DisplayManager &displayManager, TimerLogic &timerLogic);

  void begin();
  void handleWireless();

 private:
  DisplayManager &display;
  TimerLogic &timer;
  String lineBuffer;

  void handleCommand(const String &cmd);
  void handleStatusCommand(const String &upper, const String &original);
  void handleTimerCommand(const String &cmdTail);
  void handleEmojiCommand(const String &name);
  void printHelp() const;
};

#endif


