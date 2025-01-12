#ifndef SWCtrl_h
#define SWCtrl_h
#include <Arduino.h>

class SWCtrl {
  public:
    void init();
    void loop();

  private:
    void adjustDac(int voltArray[], bool isThrottle = false);
    void adjustThrottle();
    void adjustSteering();
};

#endif
