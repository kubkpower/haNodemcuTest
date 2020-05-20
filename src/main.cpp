#include <Arduino.h>
#include "simpleUI.h"
#include "haMqttCtrl.h"
#include "haNodeDht.h"
#include "haSensorSwitch.h"
#include "haNodeButton.h"
#include "haSensorGeneric.h"
#include "haSensorSignalStrength.h"
#include "haSensorTimestamp.h"
#include "haNodeStatus.h"

#define DHT_PIN   16
#define RELAY_PIN D3
#define BTN_PIN   D4 

int iteration = 0;
simpleUI *ecran;
haMqttCtrl *ctrl;

haNodeDht nodeDht;
haNodeButton nodeButton;

haSensorSwitch swSensor(RELAY_PIN);
haSensorGeneric test;
haSensorSignalStrength wifiSensor;
haSensorTimestamp uptimeSensor;
haNodeStatus myStatus;
String startupTime;

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived on [");
  Serial.print(topic);
  Serial.println("]");
  
  ecran->blinkRx();
  swSensor.processPayload(topic, payload, length);
  //nodeHum.processPayload(topic, payload, length);
  //nodeTemp.processPayload(topic, payload, length);
}

ICACHE_RAM_ATTR void clickCallback() {
  //Serial.println("click");
  nodeButton.clickCallback(BTN_PIN);
}

void onClick(int clickCount) {
  Serial.print("Click count = ");
  Serial.println(clickCount); 
  switch (clickCount) {
    case 1:
      swSensor.flip();
      break;
    case 2:
      ecran->displayStatus();
      break;
    default:
      Serial.println("Click ignored");
  }
}

void setup()
{
  Serial.begin(115200);
  // initialize LED digital pin as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  ecran = new simpleUI();
  //ecran->displayDemoScreen();
  ctrl = new haMqttCtrl(ecran);
  ctrl->init();
  ctrl->setCallback(mqttCallback);
  ctrl->loop();
  nodeDht.configure(ctrl, DHT_PIN);

  // Switch Sensor
  swSensor.configure(ctrl, String(ESP.getChipId()));
  swSensor.subscribeNode();

  pinMode(BTN_PIN ,INPUT_PULLUP);
  nodeButton.setCallbackOnClick(onClick);
  attachInterrupt(digitalPinToInterrupt(BTN_PIN), clickCallback, FALLING);

  test.configure(ctrl, "status", String(ESP.getChipId()));
  test.setValue("Test 1");

  wifiSensor.configure(ctrl, String(ESP.getChipId()));
  uptimeSensor.configure(ctrl, String(ESP.getChipId()));

  
  Serial.print("Startup Epoch time: ");
  Serial.println(startupTime);

  String myIP = WiFi.localIP().toString();
  ecran->setInfos(0,(String("SSID: ")+String(WiFi.SSID())));
  ecran->setInfos(1,(String("IP: ") + myIP));
  ecran->setInfos(2,String("Started since"));
  ecran->setLabel(0, "Temperature");
  ecran->setLabel(1, "Humidity");
}

void loop()
{
  //ecran->runDemo();
  ctrl->loop();
  ecran->loop();

  // Retrieve the startuptime asap
  if (startupTime == "") {
    if (!myStatus.timeAvailable)
      startupTime = myStatus.getEpochTime();
      //
      String strTime = myStatus.convertEpochToDate(startupTime);
      Serial.print("Startup time: "); Serial.println(strTime);
      ecran->setInfos(3,strTime);
  }

  nodeButton.loop();
  if (nodeDht.refreshData()) {
    Serial.print("Temperature : ");
    Serial.println((String) nodeDht.getTemperature());
    ecran->setText(0, nodeDht.getTemperatureString());
    Serial.print("Humidite : ");
    Serial.println((String) nodeDht.getHumidity());
    ecran->setText(1, nodeDht.getHumidityString());

    wifiSensor.setValue(WiFi.RSSI());
    //delay(500);

    // Get startup time if internet available and connected   
    
    myStatus.loop();
    if (startupTime != "") {
      uptimeSensor.setValue(startupTime);
    }
    
  }
  /*
  Serial.print("Iteration: ");
  Serial.println(iteration);
  testScreen();
  iteration++;
  */
} 

