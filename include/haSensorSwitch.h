#include "haSensor.h"

#ifndef _haSensorSwitch_h
#define _haSensorSwitch_h

class haSensorSwitch : public haSensor {
  public:
    haSensorSwitch();
    haSensorSwitch(uint8_t relayPin);
    void setState(bool swState);
    void flip();
    bool processPayload(char* topic, byte* payload, unsigned int length);
    String getKey() { return String(); }
  protected:
    void init();
  private:
    String buildConfigTopic();
    String buildConfigPayload();
    String buildStateTopic();
    String buildCommandTopic();
    String buildUnit() { return String(); }
    void setRelay();
    uint8_t state;
    uint8_t relayPin;
    

    // payloads by default (on/off)
    const char* SW_ON = "ON";
    const char* SW_OFF = "OFF";
};

#endif
