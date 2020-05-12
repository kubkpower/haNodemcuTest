#include "haNodeButton.h"

haNodeButton::haNodeButton() {
  isDebouncing = false;
  isBtnPressed = false;
  pressTime = 0;
  ackTime = 0;
  clickCount = 0;
}

void haNodeButton::clickCallback(int btnPin) {
  if (millis() > (pressTime + CLICK_DEBOUNCE)) {
    // Detect push action
    
    Serial.println("clickCallback");
    // Press button
    
    // Manage btn duration and combination storage
    clickCount++;
    //
    pressTime = millis();
    ackTime = millis();
    
  }
}

void haNodeButton::loop() {
  // if clicks appened and time interval elapsed
  if ((ackTime != 0) && (millis() > (ackTime + CLICK_ACK))) {
    ackTime = 0;
    _onClickCallback(clickCount);
    // Reset code once processed
    clickCount = 0;
  }
}

void haNodeButton::setCallbackOnClick(void (*onClickCallback)(int))
{
  _onClickCallback = onClickCallback;
}
