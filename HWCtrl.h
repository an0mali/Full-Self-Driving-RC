#ifndef HWCtrl_h
#define HWCtrl_h
#include <Arduino.h>

class HWCtrl {
  public:
    void init();

  private:

    void setNeutralPositions();
    void setThrottleVoltage(float volts);
    void setSteerVoltage(float volts);

    void toggleRemote();
    
    void testSteering();
    void testThrottle();
    void testVolts();

    float readToVolts(int aPin);
    int convertDacVoltage(float volts);



};

#endif