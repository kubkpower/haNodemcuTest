#include "haSensorGeneric.h"
#include <string.h>

String haSensorGeneric::buildConfigTopic() {
  String myStr = String("homeassistant/sensor/");
  myStr += String(node_name);
  myStr += String(node_id);
  myStr += String("/config");
  #ifdef _haSensorTemp_debug
  Serial.print("-- buildConfigTopic: ");
  Serial.println(myStr);
  #endif
  return myStr;
}
String haSensorGeneric::buildStateTopic() {
  String myStr = String("kubiot/sensor/");
  myStr += String(node_name);
  myStr += String(node_id);
  myStr += String("/state");
  #ifdef _haSensorTemp_debug
  Serial.print("-- buildStateTopic: ");
  Serial.println(myStr);
  #endif
  return myStr;
}

String haSensorGeneric::buildConfigPayload() {
  String myStr = String("{\"name\": \"Kubiot ");
  myStr += String(node_id);
  myStr += String(node_name);
  myStr += String("\", \"state_topic\": \"");
  myStr += String(state_topic);
  myStr += String("\", \"value_template\": \"{{value_json.value}}\",\"uniq_id\": \"");
  myStr += String(node_id);
  myStr += String("_ESP8266_");
  myStr += String(node_name);
  myStr += String("\",\"device\": {\"identifiers\": [\"");
  myStr += String(node_id);
  myStr += String("\"]}}");
  #ifdef _haSensorTemp_debug
  Serial.print("-- buildConfigPayload: ");
  Serial.println(myStr);
  #endif
  return myStr;
}
