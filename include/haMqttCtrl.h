#include <ESP8266WiFi.h>          //ESP8266 Core WiFi Library (you most likely already have this in your sketch)
#include <FS.h>                   //this needs to be first, or it all crashes and burns...
#include <DNSServer.h>            //Local DNS Server used for redirecting all requests to the configuration portal
#include <ESP8266WebServer.h>     //Local WebServer used to serve the configuration portal
#include <WiFiManager.h>
#include <ArduinoJson.h>          //https://github.com/bblanchon/ArduinoJson

#include <String.h>
#include <PubSubClient.h>
#include <simpleUI.h>
//#include "haNodeHum.h"
//#include "haNodeTemp.h"
//#include "haNodeSwitch.h"
//#include "haNodeStatus.h"

#ifndef _haMqttCtrl_h
#define _haMqttCtrl_h

#define _haMqttCtrl_debug

#define SCREEN_REFRESH_MS  5000

class haMqttCtrl {
    public:
        haMqttCtrl();  // This is the constructor
        haMqttCtrl(simpleUI *ecran);  // This is the constructor
        void init();
        void loop();
        void startConfigure();

        void sendData(String topic, String payload);
        void setCallback(MQTT_CALLBACK_SIGNATURE);
        void subscribe(String subscribeTopic);
        void reconnect();
       
   protected:
      
        PubSubClient mqtt_client;
        WiFiClient esp_client;
        WiFiManager *wifiManager;
        simpleUI *ecran;
        String node_id;
       
   private:
        bool shouldSave;
        char mqtt_server[20];
        char mqtt_port[6];
        char mqtt_login[10];
        char mqtt_pwd[10];
        WiFiManagerParameter *custom_mqtt_server;
        WiFiManagerParameter *custom_mqtt_port;
        WiFiManagerParameter *custom_mqtt_login;
        WiFiManagerParameter *custom_mqtt_pwd;
        
        void saveConfigCallback();
        
        // Manage when millis loops back to 0
        int millisLoopCount;
        int millisTimeref;

        bool isConfigured;
        int wifiLastState;
        unsigned long wifiRefreshTimeref;
};

#endif
