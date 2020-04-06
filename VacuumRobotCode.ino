#include <SharpDistSensor.h>
#include "Motors.h"

const float voltageBatCharged = 12.68; // Voltage measured when battery fully charged //Change this
const int motorPwm = 60;
const byte nbSensors = 2;
// Window size of the median filter (odd number, 1 = no filtering)
const byte medianFilterWindowSize = 5;

int bumperState = 0;
int collisionCounter = 0;

////////////PINS////////////////

// Define the array of SharpDistSensor objects
SharpDistSensor sensorArray[] = {
    SharpDistSensor(0, medianFilterWindowSize), // First sensor using pin A1
    SharpDistSensor(1, medianFilterWindowSize), // Second sensor using pin A2
                                                // Add as many sensors as required
};
uint16_t distArray[nbSensors];

//Distance Analog Sensors (Sharp)
//SharpDistSensor leftSensor(0, medianFilterWindowSize);
//SharpDistSensor rightSensor(1, medianFilterWindowSize);

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

Motors motors(3, 5, 9, 6);

void setup()
{
  Serial.begin(9600);

  //Initialize outputs and inputs
  pinMode(fanmotor, OUTPUT);
  pinMode(signalLight, OUTPUT);
  pinMode(bumper1, INPUT_PULLUP);
  pinMode(battery, INPUT);

  for (byte i = 0; i < nbSensors; i++)
  {
    sensorArray[i].setModel(SharpDistSensor::GP2Y0A51SK0F_5V_DS); // Set sensor model
    // Set other parameters as required
  }

  //leftSensor.setModel(SharpDistSensor::GP2Y0A51SK0F_5V_DS);
  //rightSensor.setModel(SharpDistSensor::GP2Y0A51SK0F_5V_DS);

  //Wait about 2 s and initialize fan if voltage ok
  waitBlinking(2, 1);
  //Crank (initialize the fan because the voltage drops when cranking)
  if (readBattery(battery) > 12.1)
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

float readBattery(int input)
{
  int readInput;
  float voltage;
  readInput = analogRead(input);
  voltage = (((readInput * 4.9) / 1000) * voltageBatCharged) / 5; // resolution of analog input = 4.9mV per Voltage
  // Serial.print(" Battery= ");
  // Serial.print(voltage);
  return voltage;
}

bool batteryControl(int input)
{
  float v_battery;
  v_battery = readBattery(input);

  // when lipo voltage is below or equal to 11.6 return false and stop robot
  return v_battery >= 11.6;
}

double sensor(int Sensor)
{
  //Returns the distance in cm
  double dist = pow(analogRead(Sensor), -0.857); // x to power of y
  return (dist * 1167.9);
}

void loop()
{
  // polling
  bumperState = digitalRead(bumper1);

  for (byte i = 0; i < nbSensors; i++)
  {
    distArray[i] = sensorArray[i].getDist();
  }

  // Serial.print(" sensor left ");
  // Serial.print(leftSensor.getDist());
  // Serial.print(" sensor right ");
  // Serial.print(rightSensor.getDist());

  if (batteryControl(battery))
  {

    digitalWrite(signalLight, HIGH);
    if (distArray[0] <= 40)
    {
      //Serial.print("trigger : ");
      //Serial.print(leftSensor.getDist());
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
    else if (distArray[1] <= 40)
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
  //Serial.println();
}
