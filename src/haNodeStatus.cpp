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
    Serial.println(timeClient.getEpochTime());
    timeAvailable = true;
    return etime;
      
  } else {
    return String();
  }
}

String haNodeStatus::convertEpochToDate(String etime) {
  struct tm  ts;
  char buf[80];
  // Format time, "ddd yyyy-mm-dd hh:mm:ss zzz"
  time_t uli = strtol(etime.c_str(),NULL,10);
  ts = *localtime(&uli);
  //strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S %Z", &ts);
  strftime(buf, sizeof(buf), "%d/%m/%y %H:%M %Z", &ts);
  return String(buf);
}
