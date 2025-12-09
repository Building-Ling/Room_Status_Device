#ifndef WIRELESS_H
#define WIRELESS_H

#include <Arduino.h>

#include "DisplayManager.h"

class WirelessHandler {
 public:
  WirelessHandler(DisplayManager &displayManager);

  void begin();
  void handleWireless();

 private:
  DisplayManager &display;
  String lineBuffer;

  void handleCommand(const String &cmd);
  void handleStatusCommand(const String &upper, const String &original);
  void handleEmojiCommand(const String &name);
  void printHelp() const;
};

#endif


