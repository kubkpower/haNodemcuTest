#include "haNodeStatus.h"

haNodeStatus::haNodeStatus() {
  timeAvailable = false;
}

void haNodeStatus::loop() {
  //d->print("SSID: "); d->println(WiFi.SSID());
  //d->print("IP: "); d->println(WiFi.localIP());
  
  
}

String haNodeStatus::getEpochTime() {
  WiFiUDP ntpUDP;
  NTPClient timeClient(ntpUDP);
  
  if (WiFi.status() == WL_CONNECTED ) {
    timeClient.begin();
    timeClient.update();
    String etime = String(timeClient.getEpochTime());
    timeClient.end();
    //Serial.println(timeClient.getEpochTime());
    timeAvailable = true;
    return etime;
      
  } else {
    return String();
  }

  
}
