#include <arduino.h>
#include "BatterySensor.h"

BatterySensor::BatterySensor(int pin, float chargedVoltage)
{
    BatterySensor = pin;
    voltageBatCharged = chargedVoltage;
    setup();
}

void BatterySensor::setup()
{
    pinMode(BatterySensor, INPUT);
}

float BatterySensor::getBatteryVoltage()
{
    int readInput;
    float voltage;
    readInput = analogRead(BatterySensor);
    voltage = (((readInput * 4.9) / 1000) * voltageBatCharged) / 5;
    return voltage;
}

bool BatterySensor::safe()
{
    float v_battery;
    v_battery = getBatteryVoltage();

    // when lipo voltage is below or equal to 11.6 return false and stop robot
    return v_battery >= 11.6;
}
