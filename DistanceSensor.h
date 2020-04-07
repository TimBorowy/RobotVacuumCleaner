#include "Sensor.h"

class DistanceSensor : public Sensor
{
public:
  int sensorPin;

  DistanceSensor(int pin);
  void setup();
  float getValue();
};