#include <SharpDistSensor.h>
#include "Motors.h"
#include "BatterySensor.h"
#include "DistanceSensor.h"

const float voltageBatCharged = 12.71; // Voltage measured when battery fully charged //Change this
const int motorPwm = 60;

int bumperState = 0;
volatile int collisionCounter = 0;

//PINS
const int sensorLeft = 0;
const int sensorRight = 1;
const int battery = 4;
const int signalLight = 13;
const int bumper1 = 2;
const int fanmotor = 12;

// Motor2 Left
const int motor2Pin1 = 3;
const int motor2Pin2 = 5;
// Motor1 Right
const int motor1Pin1 = 6;
const int motor1Pin2 = 9;

Motors motors(motor2Pin1, motor2Pin2, motor1Pin2, motor1Pin1);
BatterySensor batterySensor(battery, voltageBatCharged);
DistanceSensor distanceLeft(sensorLeft);
DistanceSensor distanceRight(sensorRight);

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

void setup()
{
  Serial.begin(9600);

  //Initialize outputs and inputs
  pinMode(fanmotor, OUTPUT);
  pinMode(signalLight, OUTPUT);
  pinMode(bumper1, INPUT_PULLUP);

  // LOW because pin gets pulled to ground
  attachInterrupt(digitalPinToInterrupt(bumper1), bumperHit, LOW);

  //Wait about 2 s and initialize fan if voltage ok
  waitBlinking(2, 1);
  //Crank (initialize the fan because the voltage drops when cranking)
  if (batterySensor.getValue() > 12.1)
  {
    digitalWrite(fanmotor, HIGH);
    delay(1000);
  }
}

void bumperHit()
{
  collisionCounter = 0;
  motors.reverse(500);
  motors.moveLeft(300);
  Serial.print("interrupt bumped");
}

void loop()
{
  // polling
  //bumperState = digitalRead(bumper1);

  Serial.print(" sensor left ");
  Serial.print(distanceLeft.getValue());
  Serial.print(" sensor right ");
  Serial.print(distanceRight.getValue());

  if (batterySensor.safe())
  {

    digitalWrite(signalLight, HIGH);
    if (distanceLeft.getValue() <= 10)
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
    else if (distanceRight.getValue() <= 10)
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
    // polling code. leave in temp for review
    // else if (bumperState == 0)
    // {
    //   collisionCounter = 0;
    //   motors.reverse(500);
    //   motors.moveLeft(300);
    //   Serial.print(" Turn Left ");
    // }
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
