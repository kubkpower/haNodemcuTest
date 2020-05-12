#include "haSensor.h"

#ifndef _haSensorTimestamp_h
#define _haSensorTimestamp_h

#define _haSensorTimestamp_debug

class haSensorTimestamp : public haSensor {
  public:
    String getKey() { return String("value"); }
  private:
    String buildConfigTopic();
    String buildConfigPayload();
    String buildStateTopic();
    String buildUnit() { return String(""); }
    
};

#endif
