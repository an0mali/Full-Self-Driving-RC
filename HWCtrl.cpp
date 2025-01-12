#include <Wire.h>
#include <Adafruit_MCP4725.h>
#include "HWCtrl.h"
Adafruit_MCP4725 dacThrottle; // uses 0x61 as addr, A0 as read
Adafruit_MCP4725 dacSteer; // uses 0x60 as addr, A1 as read

int remoteOnPin = 4;//Changed from 2 to 4 so it doesnt conflict with GPS/Compass
int throttleReadPin = A0;
int steerReadPin = A1;

float steerNeutralVolts = 1.1;//1.1;
float throttleNeutralVolts = 1.15;

void HWCtrl::init() {

  Serial.println("HWCtrl: Initializing");
  delay(100); //allow other actions to finish
  dacThrottle.begin(0x61);//Throttle DAC mem addr
  dacSteer.begin(0x60); //Steering DAC meme addr
  delay(100);

  setNeutralPositions();

  toggleRemote();

  Serial.println("HWCtrl: Init complete");
  //testVolts(); Test function, do not use when remote is connected to breadboard as it will push 5v to potentiometer leads

}

void HWCtrl::runTests() {
  //Run basic steering and throttle control tests then turn off remote
  testSteering();
  testThrottle();
  toggleRemote();
}

void HWCtrl::toggleRemote() {
  char mes[] = "Toggling remote...";
  Serial.println(mes);
  pinMode(remoteOnPin, OUTPUT);
  digitalWrite(remoteOnPin, LOW);
  delay(3000);
  pinMode(remoteOnPin, INPUT);
  char mes2[] = "Remote toggle complete";
  Serial.println(mes2);

}

void HWCtrl::setNeutralPositions() {
  char mes[] = "Setting Neutral voltage outputs";
  Serial.println(mes);
  setThrottleVoltage(throttleNeutralVolts); //1.15 is netural, 1.14v - 0.55v is min-max reverse, 1.16v - 2.4v is min-max forward. Mins may be higher
  setSteerVoltage(steerNeutralVolts);//1.1v is neutral, 1.0v - 0v is min to max left, 1.2 - 2.35v is min to max right
  delay(500);
  char mes3[] = "Set neutral position complete";
  Serial.println(mes3);

}

void HWCtrl::testThrottle() {
  char mes[] = "Setting slight throttle for two seconds";
  Serial.println(mes);
  setThrottleVoltage(1.4);
  delay(2000);
  char mes2[] = "Returning throttle to neutral position";
  Serial.println(mes2);
  setThrottleVoltage(throttleNeutralVolts);
}

void HWCtrl::testSteering() {
  //Test max left and right steering
  char mes[] = "Setting max left steer for two seconds";
  Serial.println(mes);
  setSteerVoltage(0.0);
  delay(2000);
  char mes2[] = "Returning steering to neutral position";
  Serial.println(mes2);
  setSteerVoltage(steerNeutralVolts);
  delay(2000);
  char mes4[] = "Steering max right for two seconds";
  Serial.println(mes4);
  setSteerVoltage(2.5);
  delay(2000);
  char mes5[] = "Returning steering to neutral position";
  Serial.println(mes5);
  setSteerVoltage(steerNeutralVolts);
  char mes3[] = "Steering test complete";
  Serial.println(mes3);
}

void HWCtrl::testVolts() {
    int readPin = throttleReadPin;
    String voltmes = "Throttle Voltage: ";
    for (int x = 0;  x <= 1; x++) {
      if (x > 0) { // Set read pin account
        readPin = steerReadPin;
        voltmes = "Steering Voltage: ";
      }

      for (int i = 0; i <= 5; i++) {
        if (x == 0) {
          setThrottleVoltage(i);
        }
        else {
          setSteerVoltage(i);
        }
        delay(100);
        float volts = readToVolts(readPin);
        Serial.print(voltmes);
        Serial.println(volts);

      }
    }

}

float HWCtrl::readToVolts(int aPin) {
  int value = analogRead(aPin);
  float voltage = value * (5.0 / 1023.0);
  return voltage;
}

int HWCtrl::convertDacVoltage(float volts) {
  int binVolt = (volts * 4095) / 5;
  return binVolt;
}

void HWCtrl::setThrottleVoltage (float volts) {
  dacThrottle.setVoltage(convertDacVoltage(volts), false); //converts voltage to data in, second arg is whether to write to DAC EPROM for persistance
}

void HWCtrl::setSteerVoltage (float volts) {
  dacSteer.setVoltage(convertDacVoltage(volts), false); //converts voltage to data in, second arg is whether to write to DAC EPROM for persistance
}