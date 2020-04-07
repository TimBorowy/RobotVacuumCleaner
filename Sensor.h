#ifndef SENSOR_CLASS
#define SENSOR_CLASS

class Sensor
{
public:
    int pin;
    virtual void setup() = 0;
    virtual float getValue() = 0;
};

#endif