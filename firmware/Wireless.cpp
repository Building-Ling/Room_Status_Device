#include "Wireless.h"

WirelessHandler::WirelessHandler(DisplayManager &displayManager)
    : display(displayManager), lineBuffer("") {}

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

  if (upper.startsWith("EMOJI ")) {
    handleEmojiCommand(trimmed.substring(6));
  } else if (upper == "RESET_ALL") {
    // Reset to startup state: AVAILABLE status with default emoji.
    display.reset();
  } else {
    handleStatusCommand(upper, trimmed);
  }
}

void WirelessHandler::handleStatusCommand(const String &upper,
                                          const String &original) {
  if (upper == "AVAILABLE") {
    display.setEmojiByName("SMILE");
    display.transitionTo("AVAILABLE");
  } else if (upper == "BUSY") {
    display.setEmojiByName("BUSY");
    display.transitionTo("BUSY");
  } else if (upper.startsWith("MEETING")) {
    display.setEmojiByName("NEUTRAL");
    display.transitionTo("MEETING");
  } else if (upper.startsWith("STATUS ")) {
    String t = original.substring(7);
    display.transitionTo(t);
  } else {
    Serial.println("Unknown command.");
  }
}

void WirelessHandler::handleEmojiCommand(const String &name) {
  display.setEmojiByName(name);
}

void WirelessHandler::printHelp() const {
  Serial.println("Room Status Device (no timer version)");
  Serial.println();
  Serial.println("Status commands:");
  Serial.println("  AVAILABLE               Set status to AVAILABLE");
  Serial.println("  BUSY                    Set status to BUSY");
  Serial.println("  MEETING                 Set status to MEETING");
  Serial.println("  STATUS <text>           Set custom status line");
  Serial.println();
  Serial.println("Emoji commands:");
  Serial.println("  EMOJI SMILE/NEUTRAL/BUSY/SLEEP/CHECK/WARN");
  Serial.println();
  Serial.println("System commands:");
  Serial.println("  RESET_ALL               Reset to AVAILABLE with default emoji.");
}


