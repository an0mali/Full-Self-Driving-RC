#ifndef NavModule_h
#define NavModule_h
#include <Arduino.h>

class NavModule
{
  public:
    void init();
    void setNavCoord(float destinationLat, float desitnationLong);
    void navLoop();

  private:
    float getCorrectionHeading();
    float getTargetBearing();
    float bearing(float lat,float lon,float lat2,float lon2);
    void displayInfo();

};

#endif