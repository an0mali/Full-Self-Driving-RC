#include "SWCtrl.h"

SWCtrl swctrl;

void setup()
{

  char mes[] = "Initializing RC_FSD";
  Serial.println(mes);
  
  Serial.begin(9600);
  while (!Serial) {}; //wait for serial
  delay(100); //extra delay doesnt hurt
  

  swctrl.init();//Init SWCtrl, which initializes all other systems
  

}

void loop()
{
  swctrl.loop();//Run main loop, might move some of that stuff over here

}
