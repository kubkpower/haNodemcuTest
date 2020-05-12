#include "haSensorHum.h"
#include <string.h>

String haSensorHum::buildConfigTopic() {
  String myStr = String("homeassistant/sensor/hum");
  myStr += String(this->node_id);
  myStr += String("/config");
  #ifdef _haSensorHum_debug
  Serial.print("-- buildConfigTopic: ");
  Serial.print(myStr);
  #endif
  return myStr;
}
String haSensorHum::buildStateTopic() {
  String myStr = String("kubiot/sensor/hum");
  myStr += String(node_id);
  myStr += String("/state");
  #ifdef _haSensorHum_debug
  Serial.print("-- buildStateTopic: ");
  Serial.println(myStr);
  #endif
  return myStr;
}


String haSensorHum::buildConfigPayload() {
  String myStr = String("{\"device_class\": \"humidity\", \"name\": \"Kubiot ");
  myStr += String(this->node_id);
  myStr += String(" Humidity\", \"state_topic\": \"");
  myStr += String(this->state_topic);
  myStr += String("\", \"unit_of_measurement\": \"%\", \"value_template\": \"{{value_json.humidity}}\",\"uniq_id\": \"");
  myStr += String(this->node_id);
  myStr += String("_ESP8266_Humidity\",\"device\": {\"identifiers\": [\"");
  myStr += String(this->node_id);
  myStr += String("\"]}}");
  #ifdef _haSensorHum_debug
  Serial.print("-- buildConfigPayload: ");
  Serial.print(myStr);
  #endif
  return myStr;
}
