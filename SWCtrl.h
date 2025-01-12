#ifndef SWCtrl_h
#define SWCtrl_h
#include <Arduino.h>

class SWCtrl {
  public:

  private:
    void adjustDac(int voltArray[], bool isThrottle = false);
    void adjustThrottle();
    void adjustSteering();
};

#endif
