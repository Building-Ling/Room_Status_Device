#include "DisplayManager.h"

DisplayManager::DisplayManager(uint8_t rs,
                               uint8_t enable,
                               uint8_t d4,
                               uint8_t d5,
                               uint8_t d6,
                               uint8_t d7)
    : lcd(rs, enable, d4, d5, d6, d7),
      currentStatusText("AVAILABLE"),
      lastLine2Text("AVAILABLE"),
      animating(false),
      currentEmojiSlot(SLOT_SMILE),
      pacmanSpeedMs(200),
      typeInSpeedMs(150) {}

void DisplayManager::begin() {
  lcd.begin(16, 2);
  loadEmojis(lcd);
  render(TimerSnapshot());
}

void DisplayManager::render(const TimerSnapshot &timer) {
  drawStaticStatus(timer);
}

void DisplayManager::transitionTo(const String &newStatus,
                                  const TimerSnapshot &timer) {
  currentStatusText = newStatus;
  animatePacmanLine2(lastLine2Text);
  typeInLine2(timer);
  lastLine2Text = newStatus;
}

void DisplayManager::setEmojiByName(const String &name) {
  ::setEmojiByName(name, currentEmojiSlot);
  refreshEmoji();
}

void DisplayManager::refreshEmoji() {
  lcd.setCursor(15, 1);
  lcd.write(byte(currentEmojiSlot));
}

void DisplayManager::clearDisplay() {
  lcd.clear();
  lastLine2Text = "";
  currentStatusText = "";
}

void DisplayManager::update() {
  // Placeholder for non-blocking animations in the future.
}

String DisplayManager::formatTimer(unsigned long totalSeconds) const {
  unsigned long minutes = totalSeconds / 60;
  unsigned int seconds = totalSeconds % 60;

  if (minutes > 999) {
    minutes = 999;
  }

  String out = "";

  if (minutes < 10) {
    out += "  " + String(minutes);
  } else if (minutes < 100) {
    out += " " + String(minutes);
  } else {
    out += String(minutes);
  }

  out += ":";

  if (seconds < 10) {
    out += "0";
  }
  out += String(seconds);

  return out;
}

int DisplayManager::effectiveTextLen(const String &s) const {
  int n = s.length();
  if (n > 15) n = 15;
  while (n > 0 && s.charAt(n - 1) == ' ') {
    n--;
  }
  return n;
}

String DisplayManager::buildLine2Content(const TimerSnapshot &timer) const {
  const int totalCols = 15;

  bool showTimer = (timer.active && timer.mode != TIMER_OFF);

  String timePart = "";
  int timerWidth = 0;

  if (showTimer) {
    timePart = formatTimer(timer.currentSeconds);
    timerWidth = timePart.length();
  }

  String line2;

  if (!showTimer || timerWidth == 0) {
    String statusVisible = currentStatusText;
    if (statusVisible.length() > totalCols) {
      statusVisible = statusVisible.substring(0, totalCols);
    }
    line2 = statusVisible;
    while (line2.length() < totalCols) {
      line2 += " ";
    }
  } else {
    int maxStatusCols = totalCols - timerWidth;
    if (maxStatusCols < 1) maxStatusCols = 1;

    String statusVisible = currentStatusText;
    if (statusVisible.length() > maxStatusCols) {
      statusVisible = statusVisible.substring(0, maxStatusCols);
    }

    line2 = statusVisible;

    while (line2.length() < totalCols - timerWidth) {
      line2 += " ";
    }

    line2 += timePart;

    if (line2.length() > totalCols) {
      line2 = line2.substring(0, totalCols);
    } else {
      while (line2.length() < totalCols) {
        line2 += " ";
      }
    }
  }

  return line2;
}

void DisplayManager::drawStaticStatus(const TimerSnapshot &timer) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Room Status:");

  String line2 = buildLine2Content(timer);
  lcd.setCursor(0, 1);
  lcd.print(line2);

  lcd.setCursor(15, 1);
  lcd.write(byte(currentEmojiSlot));
}

void DisplayManager::animatePacmanLine2(const String &oldText) {
  animating = true;

  lcd.setCursor(0, 1);
  String t = oldText;
  if (t.length() > 15) {
    t = t.substring(0, 15);
  }
  while (t.length() < 15) {
    t += " ";
  }
  lcd.print(t);

  int len = effectiveTextLen(t);

  if (len == 0) {
    animating = false;
    return;
  }

  for (int i = 0; i < len; i++) {
    int frame = (i % 2 == 0) ? SLOT_PAC_OPEN : SLOT_PAC_CLOSED;

    lcd.setCursor(i, 1);
    lcd.write(byte(frame));

    if (i > 0) {
      lcd.setCursor(i - 1, 1);
      lcd.print(" ");
    }

    delay(pacmanSpeedMs);
  }

  lcd.setCursor(len - 1, 1);
  lcd.print(" ");

  animating = false;
}

void DisplayManager::typeInLine2(const TimerSnapshot &timer) {
  animating = true;

  String line2 = buildLine2Content(timer);

  lcd.setCursor(0, 1);
  lcd.print("               ");

  for (int i = 0; i < 15; i++) {
    lcd.setCursor(i, 1);
    lcd.print(line2.charAt(i));
    delay(typeInSpeedMs);
  }

  refreshEmoji();

  animating = false;
}


