#ifndef SWCtrl_h
#define SWCtrl_h
#include <Arduino.h>

class SWCtrl {
  public:

  private:
    adjustDac(int voltArray[], bool isThrottle = false);
    adjustThrottle();
    adjustSteering();
}

#endif
