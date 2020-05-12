#ifndef _haSensor_h
#define _haSensor_h
#include <PubSubClient.h>
#include <String.h>
#include <ArduinoJson.h>          //https://github.com/bblanchon/ArduinoJson
#include "haMqttCtrl.h"

//#define _haSensor_debug

class haSensor {
   public:
      
      haSensor();  // This is the constructor
      void configure(haMqttCtrl *mqttCtrl, String nodeName, String nodeId);
      void configure(haMqttCtrl *mqttCtrl, String nodeId);
      
      void setValue(String data);
      void setValue(unsigned long data);
      void setValue(float data);
      void setValue(int data);
      
      void onSendData(void (*func)(void));
      bool processPayload(char* topic, byte* payload, unsigned int length) { return true;};
      virtual String getKey() = 0;
      
      String getValue() { return value; }
      DynamicJsonDocument getJsonPayload();
      String getStateTopic() { return state_topic; }
      String getFomattedValue();
      
      String command_topic;
      void subscribeNode();
   protected:
      void registerNode();
      // post-configure actions
      // required for switch to send startup state after registration
      virtual void init() {}
      
      
      void sendData();
      bool node_configured;
      String node_id;
      String node_name;
      String config_topic;
      String config_payload;
      String state_topic;
      
      String value;
      String unit;
      virtual String buildUnit() = 0;
      void (*_sendcallback)(void) = NULL;
      virtual String buildConfigTopic() = 0;
      virtual String buildConfigPayload() = 0;
      virtual String buildStateTopic() { return String(); };
      virtual String buildCommandTopic() { return String(); };
      
      haMqttCtrl *mqttCtrl;
};

#endif
