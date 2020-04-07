#include <arduino.h>
#include "DistanceSensor.h"

DistanceSensor::DistanceSensor(int pin)
{
  sensorPin = pin;
  setup();
}

void DistanceSensor::setup()
{
  pinMode(sensorPin, INPUT);
}

float DistanceSensor::getValue()
{
  //Returns the distance in cm
  double dist = pow(analogRead(sensorPin), -0.857); // x to power of y
  return (dist * 1167.9);
}
