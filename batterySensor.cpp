#include <arduino.h>
#include "BatterySensor.h"

BatterySensor::BatterySensor(int pin, float chargedVoltage)
{
    sensorPin = pin;
    voltageBatCharged = chargedVoltage;
    setup();
}

void BatterySensor::setup()
{
    pinMode(sensorPin, INPUT);
}

float BatterySensor::getValue()
{
    int readInput;
    float voltage;
    readInput = analogRead(sensorPin);
    voltage = (((readInput * 4.9) / 1000) * voltageBatCharged) / 5;
    return voltage;
}

bool BatterySensor::safe()
{
    float v_battery;
    v_battery = getValue();

    // when lipo voltage is below or equal to 11.6 return false and stop robot
    return v_battery >= 11.6;
}
