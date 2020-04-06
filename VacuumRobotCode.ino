#include <SharpDistSensor.h>
#include "Motors.h"
#include "BatterySensor.h"

const float voltageBatCharged = 12.71; // Voltage measured when battery fully charged //Change this
const int motorPwm = 60;

int bumperState = 0;
int collisionCounter = 0;

//PINS
const int sensorLeft = 0;
const int sensorRight = 1;

const int battery = 4;
const int signalLight = 13;
const int bumper1 = 10;
const int fanmotor = 12;

// Motor2 Left
const int motor2Pin1 = 3;
const int motor2Pin2 = 5;
// Motor1 Right
const int motor1Pin1 = 6;
const int motor1Pin2 = 9;

Motors motors(motor2Pin1, motor2Pin2, motor1Pin2, motor1Pin1);
BatterySensor batterySensor(battery, voltageBatCharged);

void setup()
{
  Serial.begin(9600);

  //Initialize outputs and inputs
  pinMode(fanmotor, OUTPUT);
  pinMode(signalLight, OUTPUT);
  pinMode(bumper1, INPUT_PULLUP);
  pinMode(sensorLeft, INPUT);
  pinMode(sensorRight, INPUT);

  //Wait about 2 s and initialize fan if voltage ok
  waitBlinking(2, 1);
  //Crank (initialize the fan because the voltage drops when cranking)
  if (batterySensor.getBatteryVoltage() > 12.1)
  {
    digitalWrite(fanmotor, HIGH);
    delay(1000);
  }
}

void waitBlinking(int n, int frequency)
{
  //blink for n seconds at frequency hz
  for (int i = 1; i <= n; i++)
  {
    for (int j = 1; j <= frequency; j++)
    {
      digitalWrite(signalLight, HIGH);
      delay((1000 / frequency) / 2); //Half time on
      digitalWrite(signalLight, LOW);
      delay((1000 / frequency) / 2); //Half time off
    }
  }
}

// float readBattery(int input)
// {
//   int readInput;
//   float voltage;
//   readInput = analogRead(input);
//   voltage = (((readInput * 4.9) / 1000) * voltageBatCharged) / 5; // resolution of analog input = 4.9mV per Voltage
//   Serial.print(" Battery= ");
//   Serial.print(voltage);
//   return voltage;
// }

// bool batteryControl(int input)
// {
//   float v_battery;
//   v_battery = readBattery(input);

//   // when lipo voltage is below or equal to 11.6 return false and stop robot
//   return v_battery >= 11.6;
// }

double getSensorValue(int Sensor)
{
  //Returns the distance in cm
  double dist = pow(analogRead(Sensor), -0.857); // x to power of y
  return (dist * 1167.9);
}

void loop()
{
  // polling
  bumperState = digitalRead(bumper1);

  Serial.print(" sensor left ");
  Serial.print(getSensorValue(sensorLeft));
  Serial.print(" sensor right ");
  Serial.print(getSensorValue(sensorRight));

  if (batterySensor.safe())
  {

    digitalWrite(signalLight, HIGH);
    if (getSensorValue(sensorLeft) <= 10)
    {
      //If the distance between an object and the left front sensor is less than 4.3 cm or the bumper hits, it will move to the left
      if (collisionCounter == 2)
      { // prevent of being stuck on corners
        collisionCounter = 0;
      }
      //motors.forward(100);  // approach a bit
      motors.reverse(500);
      motors.moveLeft(300);
      collisionCounter += 2;
      Serial.print("  Turn Left ");
      Serial.println();
    }
    else if (getSensorValue(sensorRight) <= 10)
    {
      //If the distance between an object and the right front sensor is less than 4.3 cm, it will move to the right
      if (collisionCounter == 1)
      {
        collisionCounter = 0;
      }
      //motors.forward(100);
      motors.reverse(500);
      motors.moveRight(300);
      collisionCounter++;
      Serial.print("  Turn Right");
    }
    else if (bumperState == 0)
    {
      collisionCounter = 0;
      motors.reverse(500);
      motors.moveLeft(300);
      Serial.print("  Turn Left ");
    }
    else
    {
      if (collisionCounter == 3)
      { //Corner
        motors.moveLeft(1000);
        collisionCounter = 0;
      }
      else
      {
        motors.forward(0);
      }
      //Serial.print("  Move Forward");
    }
  }
  else
  {
    //If the battery is low, turn everything off
    digitalWrite(fanmotor, LOW); //Turn the Fan OFF

    motors.stop();
    Serial.print(" Low Battery! ");
    Serial.println();
    waitBlinking(1, 3); //blink as warning 3hz in a loop
  }
  Serial.println();
}
