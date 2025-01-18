#include "AvoidModule.h"
//AvoidModule uses ultrasound to detect collisions and logic for what to do to avoid them

const int trigPin = 9;
const int echoPin = 10;


//We probably want to issue minor steering commands if obstacle detected within long distance and major corrections at short distances

//At long distances an echo seems to get received causing obstacle distance to be inaccurate?

unsigned long pingTime;
//int pingDelay = 0;

void AvoidModule::init()
{

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

}

bool AvoidModule::checkCollision()
{
  //Dont think the delay is necessary since issue was interupt caused by arduino
 // unsigned long currentTime = millis();
 // if (currentTime - pingTime > pingDelay) {
      avoidLoop();//testing purpose
  //    pingTime = currentTime;
  //};
 
  return false;
}

void AvoidModule::avoidLoop()
{

  float distance = getCollisionDistance();
  Serial.print("Obstacle distance: ");
  Serial.println(distance);

}

float AvoidModule::getCollisionDistance()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  noInterrupts();//disable arudino interupts during this time
  float duration = pulseIn(echoPin, HIGH);
  interrupts();

  float distance = (duration * 0.0343) / 2;

  return distance;
}