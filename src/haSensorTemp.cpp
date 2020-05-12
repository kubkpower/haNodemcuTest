#include "haSensorTemp.h"
#include <string.h>

String haSensorTemp::buildConfigTopic() {
  String myStr = String("homeassistant/sensor/temp");
  myStr += String(node_id);
  myStr += String("/config");
  #ifdef _haSensorTemp_debug
  Serial.print("-- buildConfigTopic: ");
  Serial.println(myStr);
  #endif
  return myStr;
}
String haSensorTemp::buildStateTopic() {
  String myStr = String("kubiot/sensor/temp");
  myStr += String(node_id);
  myStr += String("/state");
  #ifdef _haSensorTemp_debug
  Serial.print("-- buildStateTopic: ");
  Serial.println(myStr);
  #endif
  return myStr;
}

String haSensorTemp::buildConfigPayload() {
  String myStr = String("{\"device_class\": \"temperature\", \"name\": \"Kubiot ");
  myStr += String(node_id);
  myStr += String(" Temperature\", \"state_topic\": \"");
  myStr += String(state_topic);
  myStr += String("\", \"unit_of_measurement\": \"°C\", \"value_template\": \"{{value_json.temperature}}\",\"uniq_id\": \"");
  myStr += String(node_id);
  myStr += String("_ESP8266_Temperature\",\"device\": {\"identifiers\": [\"");
  myStr += String(node_id);
  myStr += String("\"]}}");
  #ifdef _haSensorTemp_debug
  Serial.print("-- buildConfigPayload: ");
  Serial.println(myStr);
  #endif
  return myStr;
}
