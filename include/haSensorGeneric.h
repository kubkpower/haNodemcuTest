#include "haSensor.h"

#ifndef _haSensorGeneric_h
#define _haSensorGeneric_h

//#define _haSensorGeneric_debug

class haSensorGeneric : public haSensor {
  public:
    String getKey() { return String("value"); }
  private:
    String buildConfigTopic();
    String buildConfigPayload();
    String buildStateTopic();
    String buildUnit() { return String(""); }
    
};

#endif
