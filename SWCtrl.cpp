
#include "SWCtrl.h"
///////////////////////

//Control module receives steeringRequests from nav_module and obstacle_avoidance_module to adjust steering
// This module controls if vehicle is in a navToLocation or avoidObstacle state and determines which module accordingly to get steeringRequests from


//FIne tuning how the vehicle responds should be in this moduleg

//Drive module to cotrol DACs might be seperate due to complexity of this module
//This module should be human readable while DAC/drive module will convert that data into machine signals

//This module performs constant calculations and pulls sensor data from other modules but does not communicate with sensors directly


// Steering occurs with 3 intensities? 0,1,2, 3, 4, 5, 6 (hard left, left, slight left, straight, slight right, right, hard right)
// thresholds should be established for what calls for hard, medium, or slight steering adjustsments. This may also integrate with throttle state
// Hard steering reduces thorttle speed, straight increases throttle
// Steering automatically returns to 0 in steps so vehicle will straighten out if nav data isnt obtained

const float steerProportions[] = {1.0, 0.3, 0.1, 0.0, 0.1, 0.3, 1.0}; //percentage of steering emulation
const float throttleProportions[] = {0, 10.0, 10.0, 20.0, 50.0, 100.0}; // percentage of thorttle emulation

//Throttle is in 4 intensities
// 0, 1, 2, 3, 4, 5 (stop, reverse, forward crawl, forward walk, forward run, forward sprint)

// Adjustments should be gradually in steps
// Adjustment from any state to stop should adjust throttle more quickly than acceleration adjustment

///////////////////////


//// Set voltage constants
//// Could be handled with arrays

const float maxLeftVoltage = 0.0; // Set voltages according to remote potentiometer readings gathered with a multimeter, min/max refers to steering/ throttle intensity not voltage number
const float minLeftVoltage = 1.0;
const float maxRightVoltage = 2.4;
const float minRightVoltage = 1.2;
const float neutralVoltage = 1.1;

const float maxFThrottleVoltage = 2.4;
const float minFThrottleVoltage = 1.3;
const float stopThrottleVoltage = 1.2;
const float minRThrottleVoltage = 1.1;
const float maxRThrottleVoltage = 0.5;


const float steerVoltAr[] = {maxLeftVoltage, minLeftVoltage, neutralVoltage, maxRightVoltage, minRightVoltage};
const float throttleVoltAr[] = {maxFThrottleVoltage, minFThrottleVoltage, stopThrottleVoltage, maxRThrottleVoltage, minRThrottleVoltage};

const float steerRange = maxRightVoltage - maxLeftVoltage;
const float throttleRange = maxFThrottleVoltage - maxRThrottleVoltage;

const float controlResolution = 100.0; //How many steps between min and max, higher number may result in finer adjustments but may become meaningless as DAC can only do x.xxV

const float steerStep = steerRange / controlResolution; //100 is arbitrary, finer
const float throttleStep = throttleRange / controlResolution;
const float stepThresh = 0.1; //do not adjust DAC voltage if difference between cur and tar Voltages are less than this value, should prevent DAC from getting constant change commands
////

/////////////////////
//Control steering state vars
bool isAvoiding = false; // if false use navModule for steering, if true use avoidModule for steering

int steeringState = 3; //begin steering state in straight mode
int throttleState = 0; //begin throttle state in stop mode

float curThrottleVoltage = neutralVoltage;
float tarThrottleVoltage = neutralVoltage;
float throttleStepVoltAr[] = {curThrottleVoltage, curThrottleVoltage};

float curSteerVoltage = stopThrottleVoltage;
float tarSteerVoltage = stopThrottleVoltage;
float steerStepVoltAr[] = {stopThrottleVoltage, stopThrottleVoltage};
////

void SWCtrl::adjustDac(int voltArray[], bool isThrottle = false) {
  //0 is current voltage, 1 is target voltage
  //if stepThresh checks out, we know adjustment needs to be made, so only check if it needs to go down, in which case * by -1.0
  float adjust = stepThresh;
  bool isLow = false; //if going downward, check low limit, else check upper limit
  if (abs(voltArray[0] - voltArray[1]) > stepThresh)
  {
    if (voltArray[0] > voltArray[1])
    {
      adjust *= -1.0;
      isLow = true;
    }

    float newValue = voltArray[0] + adjust;

    //Run min max setting checks

    if (isThrottle)
    {
      //driveModule.adjustDacThrottle(newValue);
    }
    else
    {
      //driveModule.adjustDacSteering(newValue);
    }
    
  }
}

void SWCtrl::adjustThrottle () {
  if (abs(curThrottleVoltage - tarThrottleVoltage) > stepThresh) //Only adjust if difference is non-negligible
  {

    if (curThrottleVoltage > tarThrottleVoltage)
    {
      curThrottleVoltage -= steerStep;
    }
    else if (curThrottleVoltage < tarThrottleVoltage)
    {
      curThrottleVoltage += throttleStep;
    }

    //driveModule.adjustSteeringDAC(curSteerVoltage)

  }
}

void SWCtrl::adjustSteering () {
  if (abs(curSteerVoltage - tarSteerVoltage) > stepThresh) //Only adjust if difference is non-negligible
  {

    if (curSteerVoltage > tarSteerVoltage)
    {
      curSteerVoltage -= steerStep;
      return;
    }
    if (curSteerVoltage < tarSteerVoltage)
    {
      curSteerVoltage += steerStep;
    }

    //driveModule.adjustSteeringDAC(curSteerVoltage)

  }
}
