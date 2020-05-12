#include <DHTesp.h>
#include "haSensorTemp.h"
#include "haSensorHum.h"
#include "haMqttCtrl.h"


#ifndef _haNodeDht_h
#define _haNodeDht_h

//#define _haNodeDht_debug


#define DHT_REFRESH_RATE  300000

class haNodeDht {
  public:
    haNodeDht();  // This is the constructor
    void configure(haMqttCtrl *mqttCtrl, int dhtPin);
    bool refreshData();

    String getTemperatureString() { return haNodeDht::toString(temperature,1); }
    float getTemperature() { return temperature; }
    String getHumidityString()  { return haNodeDht::toString(humidity,1); }
    float getHumidity() { return humidity; }
     
  protected:
    
     
  private:
    bool isConfigured;
    /** Initialize DHT sensor */
    DHTesp dht;
    /** Pin number for DHT11 data pin */
    int dhtPin;
    /** Last data refresh ms **/
    unsigned long lastDataRefresh;
    /** Values **/
    float humidity;
    float temperature;
    /* toString */
    static String toString(float value, int precision);
    haSensorTemp tempSensor;
    haSensorHum humSensor;
    haMqttCtrl *mqttCtrl;
};

#endif
