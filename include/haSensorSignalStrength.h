#include "haSensor.h"

#ifndef _haSensorSignalStrength_h
#define _haSensorSignalStrength_h

#define _haSensorSignalStrength_debug

class haSensorSignalStrength : public haSensor {
  public:
    String getKey() { return String("value"); }
  private:
    String buildConfigTopic();
    String buildConfigPayload();
    String buildStateTopic();
    String buildUnit() { return String("dbm"); }
    
};

#endif
