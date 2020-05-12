#include "haSensorSignalStrength.h"
#include <string.h>

String haSensorSignalStrength::buildConfigTopic() {
  String myStr = String("homeassistant/sensor/signal_strength");
  myStr += String(node_id);
  myStr += String("/config");
  #ifdef _haSensorSignalStrength_debug
  Serial.print("-- buildConfigTopic: ");
  Serial.println(myStr);
  #endif
  return myStr;
}
String haSensorSignalStrength::buildStateTopic() {
  String myStr = String("kubiot/sensor/signal_strength");
  myStr += String(node_id);
  myStr += String("/state");
  #ifdef _haSensorSignalStrength_debug
  Serial.print("-- buildStateTopic: ");
  Serial.println(myStr);
  #endif
  return myStr;
}

String haSensorSignalStrength::buildConfigPayload() {
  String myStr = String("{\"device_class\": \"signal_strength\", \"name\": \"Kubiot ");
  myStr += String(node_id);
  myStr += String(" Signal Strength\", \"state_topic\": \"");
  myStr += String(state_topic);
  myStr += String("\", \"unit_of_measurement\": \"dbm\", \"value_template\": \"{{value_json.value}}\",\"uniq_id\": \"");
  myStr += String(node_id);
  myStr += String("_ESP8266_SignalStrength\",\"device\": {\"identifiers\": [\"");
  myStr += String(node_id);
  myStr += String("\"]}}");
  #ifdef _haSensorSignalStrength_debug
  Serial.print("-- buildConfigPayload: ");
  Serial.println(myStr);
  #endif
  return myStr;
}
