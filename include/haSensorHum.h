#include "haSensor.h"

#ifndef _haSensorHum_h
#define _haSensorHum_h

//#define _haSensorHum_debug

class haSensorHum : public haSensor {
  public:
    String getKey() { return String("humidity"); }
  private:
    String buildConfigTopic();
    String buildConfigPayload();
    String buildStateTopic();
    String buildUnit() { return String("%"); }
    
};

#endif
