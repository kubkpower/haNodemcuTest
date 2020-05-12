#include <DHTesp.h>
#include "haSensorTemp.h"
//#include "haSensorHum.h"


#ifndef _haNodeButton_h
#define _haNodeButton_h

#define _haNodeButton_debug

// Debounce delay
#define CLICK_DEBOUNCE    200
// Dalay before considering end of key combination
#define CLICK_ACK         1000
// Time limit between click and long press
#define CLICK_LIMIT       2000       

class haNodeButton {
  public:
    haNodeButton();  // This is the constructor
    void loop();
    // define callback definer
    void setCallbackOnClick(void (*onClickCallback)(int));
    void clickCallback(int btnPin);
  protected:
    
     
  private:
    bool isDebouncing;
    bool isBtnPressed;
    unsigned long pressTime;
    unsigned long ackTime;
    int clickCount;

    void (*_onClickCallback)(int);
};

#endif
