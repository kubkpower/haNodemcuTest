#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <String.h>

#ifndef _haNodeStatus_h
#define _haNodeStatus_h

class haNodeStatus  {
  public:
    haNodeStatus();
    String getEpochTime();
    
    void loop();


    bool timeAvailable;
    
  private:
    
    

};

#endif
