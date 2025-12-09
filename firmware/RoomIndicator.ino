#include "DisplayManager.h"
#include "Wireless.h"

DisplayManager display(12, 11, 5, 4, 3, 2);
WirelessHandler wireless(display);

void setup() {
  display.begin();
  wireless.begin();
}

void loop() {
  wireless.handleWireless();
  display.update();
}
