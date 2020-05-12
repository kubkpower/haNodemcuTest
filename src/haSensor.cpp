#include "haSensor.h"


haSensor::haSensor() {
  this->node_configured = false;
  
}

void haSensor::onSendData(void (*func)(void)) {
  _sendcallback = func;
}

void haSensor::configure(haMqttCtrl *mqttCtrl, String nodeName, String nodeId) {
  node_name = nodeName;
  this->mqttCtrl = mqttCtrl;
  this->unit = buildUnit();
  #ifdef _haSensor_debug
  Serial.println("- Storing Node Id");
  #endif
  this->node_id = nodeId;
  #ifdef _haSensor_debug
  Serial.println("- Generating State topic");
  #endif
  this->state_topic = buildStateTopic();
  #ifdef _haSensor_debug
  Serial.println("- Generating Command topic");
  #endif
  this->command_topic = buildCommandTopic();
  #ifdef _haSensor_debug
  Serial.println("- Generating Config topic");
  #endif
  this->config_topic = buildConfigTopic();
  #ifdef _haSensor_debug
  Serial.println("- Generating Payload topic");
  #endif
  this->config_payload = buildConfigPayload();
  #ifdef _haSensor_debug
  Serial.println("Configuring Node:");
  Serial.print("- Config Topic: ");
  Serial.println(this->config_topic);
  Serial.print("- Config Payload: ");
  Serial.println(this->config_payload);
  Serial.print("- State Topic: ");
  Serial.println(this->state_topic);
  #endif
  this->node_configured = true;
  
  this->registerNode();
  this->init();
}

// For switch & infos
void haSensor::configure(haMqttCtrl *mqttCtrl, String nodeId) {
  node_name = "Generic";
  this->mqttCtrl = mqttCtrl;
  this->unit = buildUnit();
  #ifdef _haSensor_debug
  Serial.println("- Storing Node Id");
  #endif
  this->node_id = nodeId;
  #ifdef _haSensor_debug
  Serial.println("- Generating State topic");
  #endif
  this->state_topic = buildStateTopic();
  #ifdef _haSensor_debug
  Serial.println("- Generating Command topic");
  #endif
  this->command_topic = buildCommandTopic();
  #ifdef _haSensor_debug
  Serial.println("- Generating Config topic");
  #endif
  this->config_topic = buildConfigTopic();
  #ifdef _haSensor_debug
  Serial.println("- Generating Payload topic");
  #endif
  this->config_payload = buildConfigPayload();
  #ifdef _haSensor_debug
  Serial.println("Configuring Node:");
  Serial.print("- Config Topic: ");
  Serial.println(this->config_topic);
  Serial.print("- Config Payload: ");
  Serial.println(this->config_payload);
  Serial.print("- State Topic: ");
  Serial.println(this->state_topic);
  #endif
  this->node_configured = true;
  
  this->registerNode();
  this->init();
}

void haSensor::registerNode() {
  #ifdef _haSensor_debug
  Serial.println("haSensor::registerNode()");
  #endif
  mqttCtrl->sendData(config_topic.c_str(), config_payload.c_str());
}
void haSensor::subscribeNode() {
  #ifdef _haSensor_debug
  Serial.println("haSensor::subscribeNode()");
  #endif
  mqttCtrl->subscribe(this->command_topic);

}

void haSensor::setValue(String data) {
  value = data;
  this->sendData();
}
void haSensor::setValue(float data) {
  char bufferData[5];
  dtostrf(data,3,1,bufferData);
  value = String(bufferData);
  this->sendData();
}
void haSensor::setValue(unsigned long data) {
  
  value = String(data);
  this->sendData();
}
void haSensor::setValue(int data) {
  char bufferData[5];
  dtostrf(data,3,0,bufferData);
  value = String(bufferData);
  this->sendData();
}

String haSensor::getFomattedValue() {
  String myStr = value + String(" ") + unit;
  return myStr;
}

DynamicJsonDocument haSensor::getJsonPayload() {
  DynamicJsonDocument doc(256);
  doc[this->getKey()] = value;
  return doc;
}

void haSensor::sendData() {
  String data;
  serializeJson(getJsonPayload(), data);
  #ifdef _haSensor_debug
  Serial.println("haSensor::sendData()");
  Serial.println(data);
  #endif
  mqttCtrl->sendData(state_topic, data);
}
