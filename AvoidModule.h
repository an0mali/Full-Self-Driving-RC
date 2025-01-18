#ifndef AvoidModule_h
#define AvoidModule_h
#include <Arduino.h>

class AvoidModule
{

  public:
    void init();
    void avoidLoop();
    bool checkCollision();



  private:
    float getCollisionDistance();
  
};

#endif