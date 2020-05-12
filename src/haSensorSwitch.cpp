#include "haSensorSwitch.h"
#include <string.h>

haSensorSwitch::haSensorSwitch() {
  this->node_configured = false;
  this->relayPin = LED_BUILTIN;
  pinMode(this->relayPin, OUTPUT);
  this->setRelay();
}

// Start OFF
void haSensorSwitch::init() {
  this->setState(false);
}

haSensorSwitch::haSensorSwitch(uint8_t relayPin) {
  this->node_configured = false;
  this->relayPin = relayPin;
  pinMode(this->relayPin, OUTPUT);
  this->setRelay();
}

String haSensorSwitch::buildConfigTopic() {
  String myStr = String("homeassistant/switch/light");
  myStr += String(this->node_id);
  myStr += String("/config");
  Serial.print("-- buildConfigTopic: ");
  Serial.println(myStr);
  return myStr;
}

String haSensorSwitch::buildConfigPayload() {
  String myStr = String("{\"name\": \"Kubiot ");
  myStr += String(this->node_id);
  myStr += String(" Switch\",\"uniq_id\": \"");
  myStr += String(this->node_id);
  myStr += String("_ESP8266_Relay\",\"state_topic\": \"");
  myStr += String(this->state_topic);
  myStr += String("\", \"command_topic\": \"");
  myStr += String(this->command_topic);
  myStr += String("\",\"device\": {\"identifiers\": [\"");
  myStr += String(this->node_id);
  myStr += String("\"]}}");
  Serial.print("-- buildConfigPayload: ");
  Serial.println(myStr);
  return myStr;
}

String haSensorSwitch::buildStateTopic() {
  String myStr = String("kubiot/");
  myStr += this->node_id;
  myStr += "/switch";
  Serial.print("-- buildStateTopic: ");
  Serial.println(myStr);
  return myStr;
}

String haSensorSwitch::buildCommandTopic() {
  String myStr = String("kubiot/");
  myStr += this->node_id;
  myStr += "/switch/set";
  Serial.print("-- buildCommandTopic: ");
  Serial.println(myStr);
  return myStr;
}

void haSensorSwitch::setState(bool swState) {
  this->state = swState;
  if (this->state) {
    mqttCtrl->sendData(state_topic.c_str(), SW_ON);
  } else {
    mqttCtrl->sendData(state_topic.c_str(), SW_OFF);
  }
  this->setRelay();
}

void haSensorSwitch::setRelay() {
  digitalWrite(this->relayPin, this->state);
}

void haSensorSwitch::flip() {
  this->state = !this->state;
  if (this->state) {
    mqttCtrl->sendData(state_topic.c_str(), SW_ON);
  } else {
    mqttCtrl->sendData(state_topic.c_str(), SW_OFF);
  }
  this->setRelay();
}

bool haSensorSwitch::processPayload(char* topic, byte* payload, unsigned int length) {
  if (command_topic.equals(String(topic))) {
    String pl;
    for (unsigned int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
      pl += (char)payload[i];
    }
    Serial.println("Switch topic triggered");
    
    // Switch on the LED if an 1 was received as first character
    if (pl.equals(SW_ON)) {
      Serial.println("Requested Switch state ON");
      state = true;
    } else if (pl.equals(SW_OFF)) {
      Serial.println("Requested Switch state OFF");
      state = false;
    } else {
      Serial.println("Requested Switch state Unknown");
    }
    
    this->setState(this->state);
    return true;
  }
  return false;
}
