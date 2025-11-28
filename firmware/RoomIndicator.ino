#include <LiquidCrystal.h>
#include "Emoji.h"

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

String line = "";

// current base status text (without timer)
String currentStatusText = "AVAILABLE";
// last line-2 status text used for Pac-Man to "eat"
String lastLine2Text     = "AVAILABLE";

// animation gate
bool isAnimating = false;

// current emoji shown on far right
int currentEmojiSlot = SLOT_SMILE;

// ---------- Timer state ----------

enum TimerMode {
  TIMER_OFF,
  TIMER_UP,
  TIMER_DOWN
};

TimerMode timerMode = TIMER_OFF;
bool timerActive = false;

unsigned long timerStartMillis      = 0;   // when timer started
unsigned long timerDurationSec      = 0;   // total duration for countdown
unsigned long lastTimerUpdateMillis = 0;   // last tick update
unsigned long currentTimerSeconds   = 0;   // value shown on display

// ---------- Speed control (for animations) ----------

int PACMAN_SPEED = 200;   // ms per step (you liked 200ms)
int TYPEIN_SPEED = 150;    // ms per char

// ---------- Helpers ----------

// Format seconds as "MMM:SS" (fixed width 6)
String formatTimer(unsigned long totalSeconds) {
  unsigned long minutes = totalSeconds / 60;
  unsigned int seconds  = totalSeconds % 60;

  if (minutes > 999) minutes = 999;  // clamp

  String out = "";

  if (minutes < 10)       out += "  " + String(minutes);
  else if (minutes < 100) out += " "  + String(minutes);
  else                    out +=        String(minutes);

  out += ":";

  if (seconds < 10) out += "0";
  out += String(seconds);

  return out; // length always 6
}

// visible length of a string, up to 15 chars, trimming trailing spaces
int effectiveTextLen(const String &s) {
  int n = s.length();
  if (n > 15) n = 15;
  while (n > 0 && s.charAt(n - 1) == ' ') {
    n--;
  }
  return n;
}

// Build the full 15-character line-2 content (status + optional timer),
// making sure timer fits and status is truncated if needed.
String buildLine2Content() {
  const int totalCols = 15; // cols 0..14, col 15 reserved for emoji

  // Decide if we have an active timer
  bool showTimer = (timerActive && timerMode != TIMER_OFF);

  String timePart = "";
  int timerWidth = 0;

  if (showTimer) {
    timePart = formatTimer(currentTimerSeconds); // "MMM:SS"
    timerWidth = timePart.length();              // should be 6
  }

  String line2;

  if (!showTimer || timerWidth == 0) {
    // No timer: use as much space as possible for status
    String statusVisible = currentStatusText;
    if (statusVisible.length() > totalCols) {
      statusVisible = statusVisible.substring(0, totalCols);
    }
    line2 = statusVisible;
    while (line2.length() < totalCols) line2 += " ";
  } else {
    // Timer present: pack status + timer into 15 cols.
    // We'll right-align the timer and truncate status if needed.

    int maxStatusCols = totalCols - timerWidth; // leave room for timer
    if (maxStatusCols < 1) maxStatusCols = 1;   // always at least 1 char for status

    // Take visible status portion
    String statusVisible = currentStatusText;
    if (statusVisible.length() > maxStatusCols) {
      statusVisible = statusVisible.substring(0, maxStatusCols);
    }

    line2 = statusVisible;

    // Add spaces so timer lines up to the right
    while (line2.length() < totalCols - timerWidth) {
      line2 += " ";
    }

    // Append timer
    line2 += timePart;

    // safety: ensure exact length
    if (line2.length() > totalCols) {
      line2 = line2.substring(0, totalCols);
    } else {
      while (line2.length() < totalCols) line2 += " ";
    }
  }

  return line2;
}

// ---------- Rendering ----------

void render() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Room Status:");

  String line2 = buildLine2Content();
  lcd.setCursor(0, 1);
  lcd.print(line2);

  // emoji at far right
  lcd.setCursor(15, 1);
  lcd.write(byte(currentEmojiSlot));
}

// ---------- Pac-Man animation ----------

void animatePacmanLine2(const String &oldText) {
  isAnimating = true;

  // Draw old text as it was (status-only approximation)
  lcd.setCursor(0, 1);
  String t = oldText;
  if (t.length() > 15) t = t.substring(0, 15);
  while (t.length() < 15) t += " ";
  lcd.print(t);

  int len = effectiveTextLen(t);

  if (len == 0) {
    isAnimating = false;
    return;
  }

  // Pac-Man moves only across actual text length
  for (int i = 0; i < len; i++) {
    int frame = (i % 2 == 0) ? SLOT_PAC_OPEN : SLOT_PAC_CLOSED;

    lcd.setCursor(i, 1);
    lcd.write(byte(frame));

    if (i > 0) {
      lcd.setCursor(i - 1, 1);
      lcd.print(" ");
    }

    delay(PACMAN_SPEED);   // your chosen speed
  }

  // clear last Pac-Man position
  lcd.setCursor(len - 1, 1);
  lcd.print(" ");

  isAnimating = false;
}

// Type-in animation for new line-2 content (status + timer);
// emoji appears AFTER the typing finishes.
void typeInLine2() {
  isAnimating = true;

  String line2 = buildLine2Content(); // 15 chars

  // clear line 2
  lcd.setCursor(0, 1);
  lcd.print("               "); // 15 spaces

  // type characters one by one
  for (int i = 0; i < 15; i++) {
    lcd.setCursor(i, 1);
    lcd.print(line2.charAt(i));
    delay(TYPEIN_SPEED);
  }

  // draw emoji (not typed)
  lcd.setCursor(15, 1);
  lcd.write(byte(currentEmojiSlot));

  isAnimating = false;
}

// Wrap status change in Pac-Man + type-in
void transitionTo(const String &newText) {
  // update status text
  currentStatusText = newText;

  // Pac-Man eats previous line-2 text
  animatePacmanLine2(lastLine2Text);

  // Type-in new status + timer (if any)
  typeInLine2();

  // remember for next transition
  lastLine2Text = newText;
}

// ---------- Timer logic ----------

void startTimerUp(unsigned long minutes) {
  timerMode = TIMER_UP;
  timerActive = true;
  timerStartMillis = millis();
  currentTimerSeconds = 0;
  // minutes provided is more like a "goal" mentally; we don't enforce it
  timerDurationSec = minutes * 60UL;
  lastTimerUpdateMillis = millis();
}

void startTimerDown(unsigned long minutes) {
  timerMode = TIMER_DOWN;
  timerActive = true;
  timerDurationSec = minutes * 60UL;
  timerStartMillis = millis();
  currentTimerSeconds = timerDurationSec; // start full
  lastTimerUpdateMillis = millis();
}

void stopTimer() {
  timerMode = TIMER_OFF;
  timerActive = false;
  currentTimerSeconds = 0;
}

// Call regularly in loop()
void updateTimer() {
  if (!timerActive || timerMode == TIMER_OFF) return;
  if (isAnimating) return; // don't redraw during animation

  unsigned long now = millis();
  if (now - lastTimerUpdateMillis < 250) return; // throttle checks

  unsigned long elapsedSec = (now - timerStartMillis) / 1000;
  unsigned long newSeconds = currentTimerSeconds;

  if (timerMode == TIMER_UP) {
    newSeconds = elapsedSec;
  } else if (timerMode == TIMER_DOWN) {
    if (elapsedSec >= timerDurationSec) {
      newSeconds = 0;
      timerActive = false; // done
    } else {
      newSeconds = timerDurationSec - elapsedSec;
    }
  }

  if (newSeconds != currentTimerSeconds) {
    currentTimerSeconds = newSeconds;
    render(); // update status + timer, no Pac-Man
  }

  lastTimerUpdateMillis = now;
}

// ---------- Command handling ----------

void handleCommand(String cmd) {
  cmd.trim();
  if (cmd.length() == 0) return;

  Serial.println("Received: " + cmd);

  String upper = cmd;
  upper.toUpperCase();

  // ------------ STATUS COMMANDS (NO TIMER SIDE EFFECTS) ------------

  if (upper == "AVAILABLE") {
    setEmojiByName("SMILE", currentEmojiSlot);
    transitionTo("AVAILABLE");
  }
  else if (upper == "BUSY") {
    setEmojiByName("BUSY", currentEmojiSlot);
    transitionTo("BUSY");
  }
  else if (upper.startsWith("MEETING")) {
    setEmojiByName("NEUTRAL", currentEmojiSlot);
    transitionTo("MEETING");
  }
  else if (upper.startsWith("STATUS ")) {
    String t = cmd.substring(7); // original case
    transitionTo(t);
  }

  // ------------ TIMER COMMANDS (SEPARATE FROM STATUS) ------------

  else if (upper.startsWith("TIMER ")) {
    String tail = upper.substring(6);
    tail.trim();

    if (tail.startsWith("UP")) {
      // TIMER UP <minutes>
      String rest = tail.substring(2);
      rest.trim();
      long minutes = rest.toInt();
      if (minutes <= 0) minutes = 25; // default if not given
      startTimerUp((unsigned long)minutes);
      Serial.print("Timer UP started (min): ");
      Serial.println(minutes);
      render();
    }
    else if (tail.startsWith("DOWN")) {
      // TIMER DOWN <minutes>
      String rest = tail.substring(4);
      rest.trim();
      long minutes = rest.toInt();
      if (minutes <= 0) minutes = 30; // default if not given
      startTimerDown((unsigned long)minutes);
      Serial.print("Timer DOWN started (min): ");
      Serial.println(minutes);
      render();
    }
    else if (tail == "STOP" || tail == "OFF") {
      stopTimer();
      Serial.println("Timer stopped.");
      render();
    }
    else {
      Serial.println("Unknown TIMER command.");
    }
  }

  // ------------ EMOJI / CLEAR ------------

  else if (upper.startsWith("EMOJI ")) {
    String name = cmd.substring(6);
    setEmojiByName(name, currentEmojiSlot);

    // update only emoji
    lcd.setCursor(15, 1);
    lcd.write(byte(currentEmojiSlot));
  }
  else if (upper == "CLEAR") {
    stopTimer();
    lcd.clear();
    lastLine2Text = "";
    currentStatusText = "";
  }
}

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);

  loadEmojis(lcd);

  // initial display
  render();

  Serial.println("Booted: Pac-Man + decoupled timers.");
  Serial.println("Status commands:");
  Serial.println("  AVAILABLE");
  Serial.println("  BUSY");
  Serial.println("  MEETING");
  Serial.println("  STATUS <text>");
  Serial.println("Timer commands:");
  Serial.println("  TIMER UP <minutes>");
  Serial.println("  TIMER DOWN <minutes>");
  Serial.println("  TIMER STOP or TIMER OFF");
  Serial.println("Emoji:");
  Serial.println("  EMOJI SMILE/NEUTRAL/BUSY/SLEEP/CHECK/WARN");
  Serial.println("Other:");
  Serial.println("  CLEAR");
}

void loop() {
  // Handle serial input
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n') {
      handleCommand(line);
      line = "";
    } else if (c != '\r') {
      line += c;
    }
  }

  // Timer update
  updateTimer();
}
