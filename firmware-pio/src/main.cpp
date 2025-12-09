#include <Arduino.h>
#include <LiquidCrystal.h>
#include "DisplayManager.h"
#include "Emoji.h"
#include "Wireless.h"

const uint8_t SWITCH_PIN = 6;
bool lastSwitchState = HIGH;  // because of INPUT_PULLUP

DisplayManager display(12, 11, 5, 4, 3, 2);
WirelessHandler wireless(display);

void onSwitchStatusChangedToAvailable() {
  wireless.handleCommand("AVAILABLE");
}

void onSwitchStatusChangedToBusy() {
  wireless.handleCommand("BUSY");
}

void setup() {
  display.begin();
  wireless.begin();
  
  pinMode(SWITCH_PIN, INPUT_PULLUP);
  lastSwitchState = digitalRead(SWITCH_PIN);
  
  // Apply initial switch state
  if (lastSwitchState == HIGH) {
    onSwitchStatusChangedToAvailable();
  } else {
    onSwitchStatusChangedToBusy();
  }
}

void loop() {
  wireless.handleWireless();
  
  // Check switch state
  bool reading = digitalRead(SWITCH_PIN);
  if (reading != lastSwitchState) {
    lastSwitchState = reading;
    
    if (reading == HIGH) {
      // Switch position mapped to AVAILABLE
      onSwitchStatusChangedToAvailable();
    } else {
      // Switch position mapped to BUSY
      onSwitchStatusChangedToBusy();
    }
  }
  
  display.update();
}
