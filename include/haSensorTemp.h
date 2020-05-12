#include "haSensor.h"

#ifndef _haSensorTemp_h
#define _haSensorTemp_h

//#define _haSensorTemp_debug

class haSensorTemp : public haSensor {
  public:
    String getKey() { return String("temperature"); }
  private:
    String buildConfigTopic();
    String buildConfigPayload();
    String buildStateTopic();
    String buildUnit() { return String("C"); }
    
};

#endif
