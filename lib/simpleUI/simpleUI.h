#include <Adafruit_GFX.h>    // Core graphics library
//#include <Adafruit_ST7735.h> // Hardware-specific library for ST7735
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>
#include <Wire.h>
#include "Time.h"
#include <String.h>
#include "icons.h"

#ifndef __SIMPLEUI_H__
#define __SIMPLEUI_H__

#define TFT_DC    D1     // TFT DC  pin is connected to NodeMCU pin D1 (GPIO5)
#define TFT_RST   D2     // TFT RST pin is connected to NodeMCU pin D2 (GPIO4)
#define TFT_CS    D8     // TFT CS  pin is connected to NodeMCU pin D8 (GPIO15)

// Meter colour schemes
#define RED2RED 0
#define GREEN2GREEN 1
#define BLUE2BLUE 2
#define BLUE2RED 3
#define GREEN2RED 4
#define RED2GREEN 5


#define STATUS_DISPLAY_TIMEOUT  7000
#define OLED_BLINK_TIME_MS   300

#define INFOS_LIST_MAX  6
#define INFOS_DATA_MAX  2

enum mqttState: int {
  MQTT_STATE_NOTCONNECTED,
  MQTT_STATE_CONNECTED,
  MQTT_STATE_DISCONNECTED
};

enum wifiState: int {
  WIFI_STATE_AP_MODE,
  WIFI_STATE_CONNECTED,
  WIFI_STATE_NOTCONNECTED
};

class simpleUI {
  public:
    simpleUI();
    void setWifiState(wifiState state);
    void setWifiLevel(int level);
    
    void setBatteryLevel(int level);
    
    void setPlugOn()   { d->drawBitmap(170, 0, bmp_plug, 24, 24, ST77XX_WHITE); }
    void setPlugOff()  { d->drawBitmap(170, 0, bmp_plug, 24, 24, ST77XX_BLACK); }

    void blinkRx() { set_blink(bmp_arrowDown, 140, 0, ST77XX_WHITE); oledBlinkRxOn = true; oledBlinkTimerefRx = millis(); }
    void blinkTx() { set_blink(bmp_arrowUp, 120, 0, ST77XX_WHITE); oledBlinkTxOn = true; oledBlinkTimerefTx = millis(); }
    
    void setMqttState(mqttState state);
    
    void loop();
    
    void setLabel(int id, String msg);
    void setText(int id, String msg);
    void setInfos(int id, String msg);
    
    void displayStatus() { showStatus = true; }

    void displayDemoScreen();
    void runDemo();
    
  protected:
    void draw_data(bool forceRedraw);
    void draw_settings();
    
  private:
    bool isInitialized;
    // to store settings to display infos
    String infos[INFOS_LIST_MAX];

    void set_txOff()   { set_blink(bmp_arrowUp, 120, 0, ST77XX_BLACK); oledBlinkTxOn = false; }
    void set_rxOff()   { set_blink(bmp_arrowDown, 140, 0, ST77XX_BLACK); oledBlinkRxOn = false; }
    
    
    unsigned long oledBlinkTimerefTx;
    unsigned long oledBlinkTimerefRx;
    unsigned long oledRefreshStatusTimeref;
    bool oledBlinkTxOn;
    bool oledBlinkRxOn;
    
    bool showStatus;

    String label[INFOS_DATA_MAX];
    String str[INFOS_DATA_MAX];
    String strOld[INFOS_DATA_MAX];
    
    // Battery
    int lastBatteryLevel;
    bool batteryEnabled;
    // Last Wifi Level
    int lastRange;
    int lastConnectionState;
    
    char timeBuf[20];

    //#ifdef _ADAFRUIT_ST7789H_
    Adafruit_ST7789 *d;
    //#endif
    
    void set_blink(const uint8_t *bitmap, uint8_t x, uint8_t y, uint16_t color);
   
};

#endif 
