#include "haNodeDht.h"

haNodeDht::haNodeDht() {
  isConfigured = false;
  lastDataRefresh = 0;
  //
  humidity = 0;
  temperature = 0;

  refreshData();
  //
  
}

void haNodeDht::configure(haMqttCtrl *mqttCtrl, int dhtPin){
  this->dhtPin = dhtPin;
  dht.setup(dhtPin, DHTesp::DHT22);
  isConfigured = true;
  // Declare Sensor
  tempSensor.configure(mqttCtrl, String(ESP.getChipId()));
  humSensor.configure(mqttCtrl, String(ESP.getChipId()));
}

bool haNodeDht::refreshData() {
  // Do not query data if sensor not initialized
  if (!isConfigured)
    return false;
  // Refresh data periodically
  if ((lastDataRefresh == 0) || ((millis() - lastDataRefresh) > DHT_REFRESH_RATE)) {
    //delay(dht.getMinimumSamplingPeriod());
    humidity = dht.getHumidity();
    temperature = dht.getTemperature();
    //
    humSensor.setValue(humidity);
    tempSensor.setValue(temperature);
    //
    lastDataRefresh = millis();
    // informs the data were refresh during this call
    return true;
  }
  return false;
  
}

String haNodeDht::toString(float value, int precision) {
  char bufferData[5 + precision];
  dtostrf(value,3,precision,bufferData);
  return String(bufferData);
}
