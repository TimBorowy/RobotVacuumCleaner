#include "Sensor.h"

class BatterySensor : public Sensor
{
public:
    int BatterySensor;
    float voltageBatCharged;

    BatterySensor(int pin, float chargedVoltage);
    void setup();
    float getBatteryVoltage();
    bool safe();
};