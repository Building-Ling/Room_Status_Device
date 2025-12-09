#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Arduino.h>
#include <LiquidCrystal.h>

#include "Emoji.h"

class DisplayManager {
 public:
  DisplayManager(uint8_t rs,
                 uint8_t enable,
                 uint8_t d4,
                 uint8_t d5,
                 uint8_t d6,
                 uint8_t d7);

  void begin();

  void render();
  void transitionTo(const String &newStatus);

  void reset();

  void setEmojiByName(const String &name);
  void refreshEmoji();
  void clearDisplay();

  void update();

  const String &currentStatus() const { return currentStatusText; }
  int currentEmoji() const { return currentEmojiSlot; }
  bool isAnimating() const { return animating; }

 private:
  LiquidCrystal lcd;

  String currentStatusText;
  String lastLine2Text;
  bool animating;
  int currentEmojiSlot;

  unsigned int pacmanSpeedMs;
  unsigned int typeInSpeedMs;

  String buildLine2Content() const;
  int effectiveTextLen(const String &s) const;

  void animatePacmanLine2(const String &oldText);
  void typeInLine2();
  void drawStaticStatus();
};

#endif


