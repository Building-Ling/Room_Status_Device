#ifndef EMOJI_H
#define EMOJI_H

#include <Arduino.h>
#include <LiquidCrystal.h>

// 0..5: normal emojis
enum EmojiSlot {
  SLOT_SMILE   = 0,
  SLOT_NEUTRAL = 1,
  SLOT_BUSY    = 2,
  SLOT_SLEEP   = 3,
  SLOT_CHECK   = 4,
  SLOT_WARN    = 5,

  // 6..7: reserved for Pac-Man animation frames
  SLOT_PAC_OPEN   = 6,
  SLOT_PAC_CLOSED = 7
};

// Load all normal emojis + Pac-Man frames into LCD custom slots
void loadEmojis(LiquidCrystal &lcd);

// Set current emoji slot by name
void setEmojiByName(const String &name, int &currentEmojiSlot);

#endif
