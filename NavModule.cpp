//Module needs to connect to GPS amd compass, accept incoming coordinates, read GPS, r
//calculate the target heading to gps destination coordinates, check current heading,
//and return a steering correction to the control module

#include "NavModule.h"
//For GPS
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

//For compass
#include <Wire.h>
#include <LSM303.h> //may have an issue with this as it is not the library orignal navModule was tested with
LSM303 cmps;

// Choose two Arduino pins to use for software serial
int RXPin = 2;
int TXPin = 3;

int GPSBaud = 9600;

float destLat;
float destLng;

// Create a TinyGPS++ object
TinyGPSPlus gps;

// Create a software serial port called "gpsSerial"
SoftwareSerial gpsSerial(RXPin, TXPin);

void NavModule::init()
{
  //initialize gps
  // Start the Arduino hardware serial port at 9600 baud
  Serial.begin(9600);

  // Start the software serial port at the GPS's default baud
  gpsSerial.begin(GPSBaud);

  //initialize compass
  Wire.begin();
  cmps.init();
  cmps.enableDefault();

  Serial.println("Setup complete...");
  
  /*
  Calibration values; the default values of +/-32767 for each axis
  lead to an assumed magnetometer bias of 0. Use the Calibrate example
  program to determine appropriate values for your particular unit.
  */
  cmps.m_min = (LSM303::vector<int16_t>){-32767, -32767, -32767};
  cmps.m_max = (LSM303::vector<int16_t>){+32767, +32767, +32767};
}

void NavModule::setNavCoord(float destinationLat, float destinationLong) {
  destLat = destinationLat;
  destLng = destinationLong;
}

float NavModule::getCorrectionHeading()
{
  float targetBearing = getTargetBearing();

  //read from compass, subject that current bearing from target bearing
  cmps.read();
  float currentHeading = cmps.heading();
  float correction = currentHeading - targetBearing;

  //return how much we need to correct
  return correction;
}

void NavModule::navLoop()
{
  float cHeading = getCorrectionHeading();
  Serial.print("Need to correct heading by: ");
  Serial.println(cHeading);

}


float NavModule::getTargetBearing()
{
  // This sketch displays information every time a new sentence is correctly encoded.
  //Want this to return a wait if no gps detected so vehicle doesn't continue to drive without proper heading
  while (gpsSerial.available() > 0)
    if (gps.encode(gpsSerial.read()))
      if (gps.location.isValid())
      {
        float cbear = bearing(gps.location.lat(), gps.location.lng(), destLat, destLng);
        return cbear;
      }

  // If 5000 milliseconds pass and there are no characters coming in
  // over the software serial port, show a "No GPS detected" error
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println("No GPS detected");
    while(true);
  }
}


float NavModule::bearing(float lat,float lon,float lat2,float lon2){
    //lat and lon is current GPS location, lat2 lon2 is target gps location

    float teta1 = radians(lat);
    float teta2 = radians(lat2);
    float delta1 = radians(lat2-lat);
    float delta2 = radians(lon2-lon);

    //==================Heading Formula Calculation================//

    float y = sin(delta2) * cos(teta2);
    float x = cos(teta1)*sin(teta2) - sin(teta1)*cos(teta2)*cos(delta2);
    float brng = atan2(y,x);
    brng = degrees(brng);// radians to degrees
    brng = ( ((int)brng + 360) % 360 ); 

    Serial.print("Heading GPS: ");
    Serial.println(brng);

    return brng;


  }