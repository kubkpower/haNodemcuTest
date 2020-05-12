#include "haMqttCtrl.h"


//callback notifying us of the need to save config
void haMqttCtrl::saveConfigCallback() {
  shouldSave = true;
  
}

void haMqttCtrl::setCallback(MQTT_CALLBACK_SIGNATURE) {
  mqtt_client.setCallback(callback);
}

void haMqttCtrl::subscribe(String subscribeTopic) {
  mqtt_client.subscribe(subscribeTopic.c_str());
}

void haMqttCtrl::startConfigure() {
  if (!wifiManager->startConfigPortal()) {
      //Serial.println("failed to start config portal");
  }
  //Serial.println("Started config portal");
  ecran->setWifiState(wifiState::WIFI_STATE_AP_MODE);
  ecran->displayStatus();
}

/*
void *haMqttCtrl::mqttCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.println("] ");
  ecran.oled_rxOn();
  for (int i=0; i<haNodeCount; i++) {
    nodeList[i]->processPayload(topic, payload, length);
  }
  //nodeHum.processPayload(topic, payload, length);
  //nodeTemp.processPayload(topic, payload, length);
}
*/
haMqttCtrl::haMqttCtrl() {
  Serial.begin(115200);
  shouldSave = false;
  isConfigured = false;
  wifiLastState = wifiState::WIFI_STATE_NOTCONNECTED;
}

haMqttCtrl::haMqttCtrl(simpleUI *ecran) {
  this->ecran = ecran;
  haMqttCtrl();
}

void haMqttCtrl::init() {
  if (!isConfigured) {
    millisLoopCount = 0;
    millisTimeref = 0;
    
    //read configuration from FS json
    Serial.println("mounting FS...");
    if (SPIFFS.begin()) {
      Serial.println("mounted file system");
      if (SPIFFS.exists("/config.json")) {
        //file exists, reading and loading
        Serial.println("reading config file");
        File configFile = SPIFFS.open("/config.json", "r");
        if (configFile) {
          Serial.println("opened config file");
          //size_t size = configFile.size();
          // Allocate a buffer to store contents of the file.
          StaticJsonDocument<512> doc;
          //std::unique_ptr<char[]> buf(new char[size]);
          //configFile.readBytes(buf.get(), size);
          //DynamicJsonBuffer jsonBuffer;
          //JsonObject& json = jsonBuffer.parseObject(buf.get());
          //json.printTo(Serial);
          DeserializationError error = deserializeJson(doc, configFile);
          if (error)
            Serial.println(F("Failed to read file, using default configuration"));
          //if (json.success()) {
          //  Serial.println("\nparsed json");
          //  Serial.println("Loading MQTT Server");
            if (doc["mqtt_server"]) strcpy(mqtt_server, doc["mqtt_server"]);
            Serial.println("Loading MQTT Port");
            if (doc["mqtt_port"]) strcpy(mqtt_port, doc["mqtt_port"]);
            Serial.println("Loading MQTT Login");
            if (doc["mqtt_login"]) strcpy(mqtt_login, doc["mqtt_login"]);
            Serial.println("Loading MQTT Password");
            if (doc["mqtt_pwd"]) strcpy(mqtt_pwd, doc["mqtt_pwd"]);
          //} else {
          //  Serial.println("failed to load json config");
          //}
          configFile.close();
        }
      }
    } else {
      Serial.println("failed to mount FS");
    }
  
    WiFiManagerParameter myMqttServer("server", "MQTT Server", mqtt_server, 20);
    WiFiManagerParameter myMqttPort("port", "MQTT Port", mqtt_port, 6);
    WiFiManagerParameter myMqttLogin("login", "MQTT Login", mqtt_login, 10);
    WiFiManagerParameter myMqttPwd("pwd", "MQTT Password", mqtt_pwd, 10);
  
    custom_mqtt_server = &myMqttServer;
    custom_mqtt_port = &myMqttPort;
    custom_mqtt_login = &myMqttLogin;
    custom_mqtt_pwd = &myMqttPwd;
  
    WiFiManager wm;
    wifiManager = &wm;
    //wifiManager->setSaveConfigCallback(saveConfigCallback);
    wifiManager->addParameter(&myMqttServer);
    wifiManager->addParameter(&myMqttPort);
    wifiManager->addParameter(&myMqttLogin);
    wifiManager->addParameter(&myMqttPwd);
   
    wifiManager->autoConnect();
    delay(1000);
    char *end;
    int port = strtol(mqtt_port,&end,10);
    Serial.print("Configuring MQTT Server : ");
    Serial.print(mqtt_server); Serial.print("/"); Serial.println(port);
    mqtt_client = PubSubClient(esp_client);
    mqtt_client.setServer(mqtt_server, port);
    delay(1000);
    //mqtt_client.setCallback(mqttCallback);
    isConfigured = true;
    //reconnect();

  } else {
    Serial.println("haMqttCtrl already intialized ");
  }
}

void haMqttCtrl::sendData(String topic, String payload) {
  #ifdef _haMqttCtrl_debug
  Serial.println("haMqttCtrl::sendData");
  Serial.print("- topic: ");
  Serial.println(topic);
  Serial.print("- payload: ");
  Serial.println(payload);
  #endif
  if (!isConfigured) {
    #ifdef _haMqttCtrl_debug
    Serial.println("- Cannot send : MQTT not initialized");
    #endif
    init();
  } 
  loop();

  //int retryCount = 0;
  mqtt_client.loop();
  byte buffer[512];
  uint size = payload.length();
  payload.getBytes(buffer, 512);
  mqtt_client.beginPublish(topic.c_str(), size, false);
  for (uint i=0; i<size; i++) 
    mqtt_client.write(buffer[i]);
  mqtt_client.endPublish();
  /*while (!mqtt_client.publish(topic.c_str(), payload.c_str())) {
    Serial.print("- Failed publishing "); Serial.println(retryCount);
    if (retryCount >=3 ) return;
    retryCount++;
    delay(1000);
    mqtt_client.loop();
  }*/
  if (ecran != nullptr)
    ecran->blinkTx();
}

void haMqttCtrl::reconnect() {
  // Loop until we're reconnected
  while (!mqtt_client.connected()) {
    if (!isConfigured) { init(); }
    if (ecran != nullptr)
      ecran->setMqttState(mqttState::MQTT_STATE_DISCONNECTED);
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    String strName = String("KubIOT");
    strName += node_id;
    if (mqtt_client.connect(strName.c_str(), "client", "kubmqtt")) {
      // At first connection wait for connection to establish
      
      if (ecran != nullptr)
        ecran->setMqttState(mqttState::MQTT_STATE_CONNECTED);

      Serial.println("connected");
      
      delay(1000);
      //mqtt_client->subscribe(MQTT_LIGHT_COMMAND_TOPIC.c_str());      
      //client.subscribe(configPathLight.c_str());
      //mqtt_client->publish(configPathLight.c_str(), configPayloadLight.c_str());
      //ecran.oled_txOn();
      //
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqtt_client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(1000);
    }
  }
}

void haMqttCtrl::loop() {
  if (!mqtt_client.connected()) {
    if (ecran != nullptr)
        ecran->setMqttState(mqttState::MQTT_STATE_DISCONNECTED);
    reconnect();
  }
  mqtt_client.loop();
  if (shouldSave) {
    Serial.println("saving config");
    StaticJsonDocument<512> doc;
          
    //DynamicJsonBuffer jsonBuffer;
    //JsonObject& json = jsonBuffer.createObject();
    //read updated parameters
    strcpy(mqtt_server, custom_mqtt_server->getValue());
    strcpy(mqtt_port, custom_mqtt_port->getValue());
    strcpy(mqtt_login, custom_mqtt_login->getValue());
    strcpy(mqtt_pwd, custom_mqtt_pwd->getValue());
    doc["mqtt_server"] = mqtt_server;
    doc["mqtt_port"] = mqtt_port;
    doc["mqtt_login"] = mqtt_login;
    doc["mqtt_pwd"] = mqtt_pwd;
    File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile) {
      Serial.println("failed to open config file for writing");
    }
    
    // Serialize JSON to file
    if (serializeJson(doc, configFile) == 0) {
      Serial.println(F("Failed to write to file"));
    }

    configFile.close();
    //end save
    ESP.reset();
  }

  if (millis() > (wifiRefreshTimeref + SCREEN_REFRESH_MS)) {
    // Check Wifi displayStatus
    int mode = WiFi.getMode();
    wifiState currentWifiMode;
    switch (mode) {    
      case WiFiMode_t::WIFI_AP:
      case WiFiMode_t::WIFI_AP_STA:
        currentWifiMode = wifiState::WIFI_STATE_AP_MODE;
        break;
      case WiFiMode_t::WIFI_STA:
        currentWifiMode = wifiState::WIFI_STATE_CONNECTED;
        break;
      default:
        currentWifiMode = wifiState::WIFI_STATE_NOTCONNECTED;
        break;
    }

    if ((wifiLastState != currentWifiMode) && (ecran != nullptr)) {
      ecran->setWifiState(currentWifiMode);
      wifiLastState = currentWifiMode;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
      //ecran->setWifiState(wifiState::WIFI_STATE_CONNECTED);
      int level = WiFi.RSSI();
      ecran->setWifiLevel(level);
    }

    wifiRefreshTimeref = millis();
  }
}
