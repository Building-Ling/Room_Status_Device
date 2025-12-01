#include "Emoji.h"

// --- Normal emoji characters (5x8) ---

byte EMOJI_SMILE[8] = {
  B00000,
  B01010,
  B01010,
  B00000,
  B10001,
  B01110,
  B00000,
  B00000
};

byte EMOJI_NEUTRAL[8] = {
  B00000,
  B01010,
  B01010,
  B00000,
  B11111,
  B00000,
  B00000,
  B00000
};

byte EMOJI_BUSY[8] = {
  B00000,
  B01010,
  B01010,
  B00000,
  B11111,
  B00100,
  B01010,
  B00000
};

byte EMOJI_SLEEP[8] = {
  B00000,
  B10001,
  B00010,
  B00100,
  B01000,
  B10001,
  B11111,
  B00000
};

byte EMOJI_CHECK[8] = {
  B00000,
  B00001,
  B00010,
  B10100,
  B01000,
  B00000,
  B00000,
  B00000
};

byte EMOJI_WARN[8] = {
  B00100,
  B00100,
  B00100,
  B00100,
  B00100,
  B00000,
  B00100,
  B00000
};

// --- Pac-Man frames (slots 6 & 7) ---
// Pac-Man facing RIGHT, two-frame chomp

byte PAC_OPEN[8] = {
  B00000,
  B00111,
  B01111,
  B11110,
  B11100,
  B01111,
  B00111,
  B00000
};

byte PAC_CLOSED[8] = {
  B00000,
  B00111,
  B01111,
  B11111,
  B11111,
  B01111,
  B00111,
  B00000
};

void loadEmojis(LiquidCrystal &lcd) {
  // normal emojis
  lcd.createChar(SLOT_SMILE,   EMOJI_SMILE);
  lcd.createChar(SLOT_NEUTRAL, EMOJI_NEUTRAL);
  lcd.createChar(SLOT_BUSY,    EMOJI_BUSY);
  lcd.createChar(SLOT_SLEEP,   EMOJI_SLEEP);
  lcd.createChar(SLOT_CHECK,   EMOJI_CHECK);
  lcd.createChar(SLOT_WARN,    EMOJI_WARN);

  // pac-man frames in reserved slots
  lcd.createChar(SLOT_PAC_OPEN,   PAC_OPEN);
  lcd.createChar(SLOT_PAC_CLOSED, PAC_CLOSED);
}

void setEmojiByName(const String &name, int &currentEmojiSlot) {
  String n = name;
  n.toUpperCase();

  if      (n == "SMILE")   currentEmojiSlot = SLOT_SMILE;
  else if (n == "NEUTRAL") currentEmojiSlot = SLOT_NEUTRAL;
  else if (n == "BUSY")    currentEmojiSlot = SLOT_BUSY;
  else if (n == "SLEEP")   currentEmojiSlot = SLOT_SLEEP;
  else if (n == "CHECK")   currentEmojiSlot = SLOT_CHECK;
  else if (n == "WARN")    currentEmojiSlot = SLOT_WARN;
}
