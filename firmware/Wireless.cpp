#include "Wireless.h"

WirelessHandler::WirelessHandler(DisplayManager &displayManager,
                                 TimerLogic &timerLogic)
    : display(displayManager), timer(timerLogic), lineBuffer("") {}

void WirelessHandler::begin() {
  Serial.begin(9600);
  printHelp();
}

void WirelessHandler::handleWireless() {
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n') {
      handleCommand(lineBuffer);
      lineBuffer = "";
    } else if (c != '\r') {
      lineBuffer += c;
    }
  }
}

void WirelessHandler::handleCommand(const String &cmd) {
  String trimmed = cmd;
  trimmed.trim();
  if (trimmed.length() == 0) {
    return;
  }

  Serial.println("Received: " + trimmed);

  String upper = trimmed;
  upper.toUpperCase();

  if (upper.startsWith("TIMER ")) {
    handleTimerCommand(upper.substring(6));
  } else if (upper.startsWith("EMOJI ")) {
    handleEmojiCommand(trimmed.substring(6));
  } else if (upper == "CLEAR") {
    timer.stopTimer();
    display.clearDisplay();
  } else {
    handleStatusCommand(upper, trimmed);
  }
}

void WirelessHandler::handleStatusCommand(const String &upper,
                                          const String &original) {
  const TimerSnapshot snapshot = timer.getSnapshot();

  if (upper == "AVAILABLE") {
    display.setEmojiByName("SMILE");
    display.transitionTo("AVAILABLE", snapshot);
  } else if (upper == "BUSY") {
    display.setEmojiByName("BUSY");
    display.transitionTo("BUSY", snapshot);
  } else if (upper.startsWith("MEETING")) {
    display.setEmojiByName("NEUTRAL");
    display.transitionTo("MEETING", snapshot);
  } else if (upper.startsWith("STATUS ")) {
    String t = original.substring(7);
    display.transitionTo(t, snapshot);
  } else {
    Serial.println("Unknown command.");
  }
}

void WirelessHandler::handleTimerCommand(const String &cmdTail) {
  String tail = cmdTail;
  tail.trim();

  if (tail.startsWith("UP")) {
    String rest = tail.substring(2);
    rest.trim();
    long minutes = rest.toInt();
    if (minutes <= 0) minutes = 25;
    timer.startTimerUp((unsigned long)minutes);
    Serial.print("Timer UP started (min): ");
    Serial.println(minutes);
    display.render(timer.getSnapshot());
  } else if (tail.startsWith("DOWN")) {
    String rest = tail.substring(4);
    rest.trim();
    long minutes = rest.toInt();
    if (minutes <= 0) minutes = 30;
    timer.startTimerDown((unsigned long)minutes);
    Serial.print("Timer DOWN started (min): ");
    Serial.println(minutes);
    display.render(timer.getSnapshot());
  } else if (tail == "STOP" || tail == "OFF") {
    timer.stopTimer();
    Serial.println("Timer stopped.");
    display.render(timer.getSnapshot());
  } else {
    Serial.println("Unknown TIMER command.");
  }
}

void WirelessHandler::handleEmojiCommand(const String &name) {
  display.setEmojiByName(name);
}

void WirelessHandler::printHelp() const {
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


