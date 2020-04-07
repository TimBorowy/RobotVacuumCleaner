#include "Sensor.h"

class BatterySensor : public Sensor
{
public:
    int sensorPin;
    float voltageBatCharged;

    BatterySensor(int pin, float chargedVoltage);
    void setup();
    float getValue();
    bool safe();
};