#include "DisplayManager.h"
#include "TimerLogic.h"
#include "Wireless.h"

DisplayManager display(12, 11, 5, 4, 3, 2);
TimerLogic timerLogic;
WirelessHandler wireless(display, timerLogic);

void setup() {
  display.begin();
  wireless.begin();
}

void loop() {
  wireless.handleWireless();

  if (timerLogic.update(display.isAnimating())) {
    display.render(timerLogic.getSnapshot());
  }

  display.update();
}
