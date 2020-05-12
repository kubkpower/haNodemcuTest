#include "simpleUI.h"

simpleUI::simpleUI() {
  //Serial.println("simpleUI::simpleUI BEGIN");
  isInitialized = false;
  for (int i=0; i<INFOS_DATA_MAX; i++) {
    strOld[i]=String("");
    str[i]=String("");
    label[i]=String("");
  }
  for (int i=0; i<INFOS_LIST_MAX; i++) {
    infos[i]=String("");
  }

  oledBlinkTimerefTx = 0;
  oledBlinkTimerefRx = 0;
  oledRefreshStatusTimeref = 0;
  oledBlinkTxOn = false;
  oledBlinkRxOn = false;
  showStatus = false;
  batteryEnabled = false;
  // Battery
  lastRange = 0;
  lastConnectionState = wifiState::WIFI_STATE_NOTCONNECTED;

  //#ifdef _ADAFRUIT_ST7789H_
  //Serial.println("simpleUI::simpleUI INSTANTIATE");
  d = new Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
  ////Serial.println("- Screen Init Function");
  //Serial.println("simpleUI::simpleUI INIT");
  d->init(240, 240, SPI_MODE2);
  //#endif
  delay(1000);
  // if the screen is flipped, remove this command
  //tft.setRotation(2);
  //Serial.println("- Screen Filling");
  d->fillScreen(ST77XX_BLACK);
  //Serial.println("- Init Message");
  setText(0, String("Init."));
  draw_data(true);
  ////Serial.println("- Screen Init End");
  isInitialized = true;
  //Serial.println("simpleUI::simpleUI END");
}

void simpleUI::setWifiState(wifiState state) {
  d->setCursor(5,0);
  d->setTextSize(2);
  int color = ST77XX_BLACK;
  d->fillRect(0, 0, 40, 20, color);
  String txt = "";
  if (state == wifiState::WIFI_STATE_AP_MODE) { color = ST77XX_YELLOW; txt = String("AP");}
  else if (state == wifiState::WIFI_STATE_NOTCONNECTED) { color = ST77XX_RED; txt = String("?");}
  //else { d->fillRect(0, 0, 40, 20, ST77XX_BLACK); }
  d->setTextColor(color);
  d->print(txt);
}

void simpleUI::setWifiLevel(int level) {
  // If connection state change detected, redraw
  if (lastConnectionState != wifiState::WIFI_STATE_CONNECTED) {
    d->fillRect(0, 0, 40, 20, ST77XX_BLACK);
  }
  // Compute actual level
  int range = 0;
  if (level > -80) 
    range = 1;
  if (level > -70) 
    range = 2;
  if (level > -67) 
    range = 3;
  // Check if required to redraw
  
  for (int i=0; i<=range; i++) {
    d->fillRect(10*i, (3-i)*6, 8, (i*6)+2, ST77XX_WHITE);
  }
  for (int i=(range+1); i<=3; i++) {
    d->fillRect(10*i, (3-i)*6, 8, (i*6)+2, ST77XX_BLACK);
  }
  lastRange = range;
}

void simpleUI::setBatteryLevel(int level) {
  // Set battery level at scale for display
  int batteryOffsetX = 200;
  int batteryOffsetY = 3;
  int batteryScreenSize = 30;
  int batteryScreenLevel = level * batteryScreenSize / 100;
  // Draw battery outline the first OLED_BLINK_TIME_MS
  if (!batteryEnabled) {
    batteryEnabled = true;
    d->drawRect(batteryOffsetX, batteryOffsetY, batteryScreenSize+2, 15, ST77XX_WHITE);
    d->fillRect(batteryOffsetX+batteryScreenSize+2, batteryOffsetY+3, 2, 8, ST77XX_WHITE);
  }
  //
  int color =ST77XX_GREEN;
  if (level < 20) color = ST77XX_YELLOW;
  if (level < 10) color = ST77XX_RED;

  if (level > 0) {  d->fillRect(batteryOffsetX+1, batteryOffsetY+1, batteryScreenLevel, 12, color); }
  if (level <100) { d->fillRect(batteryOffsetX+batteryScreenLevel+1, batteryOffsetY+1, batteryScreenSize-batteryScreenLevel, 12, ST77XX_BLACK); }
  
  lastBatteryLevel = level;
}

void simpleUI::setMqttState(mqttState state) {
  d->setCursor(60,4);
  d->setTextSize(2);
  int color = ST77XX_BLACK;
  if (state == mqttState::MQTT_STATE_NOTCONNECTED) { color = ST77XX_RED; }
  else if (state == mqttState::MQTT_STATE_CONNECTED) { color = ST77XX_GREEN; }
  else { color = ST77XX_YELLOW; }
  d->setTextColor(color);
  d->print("MQTT");
}

void simpleUI::draw_data(bool forceRedraw=false) {
  if ((oledRefreshStatusTimeref!= 0) && ((millis() > (oledRefreshStatusTimeref + STATUS_DISPLAY_TIMEOUT)))) {
    d->fillRect(0, 24, 240, 216, ST77XX_BLACK);
    // to prevent screen flickring
    oledRefreshStatusTimeref = 0;
    Serial.println("Clear screen");
  }
  
  for (int i=0; i<INFOS_DATA_MAX; i++) {
    ////Serial.print("simpleUI::draw_data : ");
    ////Serial.println(str[i]);
    if (strOld[i] != str[i]) {
      Serial.print("rewrite screen data ");
      Serial.println(i);
      d->setTextSize(3);
      d->setCursor(0,52 + (i*85));
      d->setTextColor(ST77XX_WHITE);
      d->println(label[i].c_str());

      d->setTextSize(5);
      d->setCursor(0,84 + (i*85));
      d->setTextColor(ST77XX_BLACK);
      d->println(strOld[i].c_str());
      d->setCursor(0,84 + (i*85));
      d->setTextColor(ST77XX_WHITE);
      d->println(str[i].c_str());

      strOld[i] = str[i];
    }
  }
}

void simpleUI::loop() {
  if (showStatus) { draw_settings(); }
  else { 
    if ((oledRefreshStatusTimeref==0) || (millis() > (oledRefreshStatusTimeref + STATUS_DISPLAY_TIMEOUT))) {
      draw_data(true);
    }
  }
  // Shut blinking icons after delay
  if (oledBlinkRxOn && (millis() > (oledBlinkTimerefRx + OLED_BLINK_TIME_MS))) { 
    set_rxOff(); 
  }
  if (oledBlinkTxOn && (millis() > (oledBlinkTimerefTx + OLED_BLINK_TIME_MS))) { 
    set_txOff(); 
  }
}

void simpleUI::setLabel(int id, String msg) {
  if ((id >=0) && (id <INFOS_DATA_MAX)) {
    label[id] = msg;
  }
}

void simpleUI::setText(int id, String msg) {
  if ((id >=0) && (id <INFOS_DATA_MAX)) {
    strOld[id] = str[id];
    str[id] = msg;
  }
}

void simpleUI::setInfos(int id, String msg) {
  if ((id >=0) && (id <INFOS_LIST_MAX)) {
    infos[id] = msg;
  }
}



void simpleUI::draw_settings() {
  //
  //Serial.println("Draw Infos");
  d->setTextSize(2);
  d->setCursor(0,32);
  d->setTextColor(ST77XX_WHITE);
  d->fillRect(0, 24, 240, 216, ST77XX_BLACK);
  
  for (int i=0; i < INFOS_LIST_MAX; i++) {
    d->println(infos[i]);
  }
   showStatus = false;
  oledRefreshStatusTimeref = millis();
}

void simpleUI::set_blink(const uint8_t *bitmap, uint8_t x, uint8_t y, uint16_t color) {
  d->drawBitmap(x, y, bitmap, 16, 16, color);
  //d->display();
}

void simpleUI::displayDemoScreen() {
  setWifiState(wifiState::WIFI_STATE_CONNECTED);
  setWifiLevel(0);

  setPlugOn();
  setBatteryLevel(19);

  setMqttState(mqttState::MQTT_STATE_CONNECTED);

  this->blinkRx(); oledBlinkRxOn=false;
  this->blinkTx(); oledBlinkTxOn=false;

  setLabel(0, "Temperature");
  setText(0, "23.5 " + String(char(247)) + "C");
  setLabel(1, "Humidity");
  setText(1, "58.2 %");
  draw_data(true);
  
}

void simpleUI::runDemo() {
  for (int i=0; i<10; i++) {
    String info = String("Info ") + String(i);
    setInfos(i,info);
  }

  Serial.println("Test Wifi");
  // Test Wifi
  setWifiState(wifiState::WIFI_STATE_AP_MODE);
  delay(500);
  setWifiState(wifiState::WIFI_STATE_NOTCONNECTED);
  delay(500);
  setWifiState(wifiState::WIFI_STATE_CONNECTED);
  delay(500);
  setWifiLevel(-100);
  delay(500);
  setWifiLevel(-75);
  delay(500);
  setWifiLevel(-68);
  delay(500);
  setWifiLevel(-50);
  delay(500);
  setWifiLevel(-68);
  delay(500);
  setWifiLevel(-75);
  delay(500);
  setWifiLevel(-100);
  delay(500);

  // Test Battery
  Serial.println("Test Battery");
  for (int i=0; i<=100; i++) { setBatteryLevel(i); delay(10); }
  for (int i=100; i>=0; i--) { setBatteryLevel(i); delay(10); }

  // Test MQTT
  Serial.println("Test MQTT");
  setMqttState(mqttState::MQTT_STATE_NOTCONNECTED);
  delay(1000);
  setMqttState(mqttState::MQTT_STATE_DISCONNECTED);
  delay(1000);
  setMqttState(mqttState::MQTT_STATE_CONNECTED);
  delay(1000);
  
  // Plug
  Serial.println("Test Plug");
  setPlugOn();
  delay(1000);
  setPlugOff();
  delay(1000);
  
  // RX/TX
  Serial.println("Test RX/TX");
  blinkRx();
  for(int i=0; i<10; i++) { loop(); delay(50); }
  blinkTx();
  for(int i=0; i<10; i++) { loop(); delay(50); }
  delay(1000);
  // Data
  Serial.println("Test Datas");
  setText(0, "Text 0");
  setText(1, "Text 1");
  loop();
  delay(1000);
  setText(0, "Text 2");
  setText(1, "Text 3");
  loop();
  delay(1000);
  setText(0, "Text 4");
  setText(1, "Text 5");
  loop();
  delay(1000);
  

  // Info
  Serial.println("Test Infos");
  displayStatus();
  for(int i=0; i<10; i++) { loop(); delay(1000); }
}
