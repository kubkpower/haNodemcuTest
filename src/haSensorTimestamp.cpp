#include "haSensorTimestamp.h"
#include <string.h>

String haSensorTimestamp::buildConfigTopic() {
  String myStr = String("homeassistant/sensor/timestamp");
  myStr += String(node_id);
  myStr += String("/config");
  #ifdef _haSensorTimestamp_debug
  Serial.print("-- buildConfigTopic: ");
  Serial.println(myStr);
  #endif
  return myStr;
}
String haSensorTimestamp::buildStateTopic() {
  String myStr = String("kubiot/sensor/timestamp");
  myStr += String(node_id);
  myStr += String("/state");
  #ifdef _haSensorTimestamp_debug
  Serial.print("-- buildStateTopic: ");
  Serial.println(myStr);
  #endif
  return myStr;
}

String haSensorTimestamp::buildConfigPayload() {
  String myStr = String("{\"device_class\": \"timestamp\", \"name\": \"Kubiot ");
  myStr += String(node_id);
  myStr += String(" timestamp\", \"state_topic\": \"");
  myStr += String(state_topic);
  myStr += String("\", \"unit_of_measurement\": \"\", \"value_template\": \"{{float(value_json.value) |timestamp_local }}\",\"uniq_id\": \"");
  myStr += String(node_id);
  myStr += String("_ESP8266_timestamp\",\"device\": {\"identifiers\": [\"");
  myStr += String(node_id);
  myStr += String("\"]}}");
  #ifdef _haSensorTimestamp_debug
  Serial.print("-- buildConfigPayload: ");
  Serial.println(myStr);
  #endif
  return myStr;
}
