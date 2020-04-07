#include <arduino.h>
#include "Motors.h"

Motors::Motors(int m1p1, int m1p2, int m2p1, int m2p2)
{
    motor1Pin1 = m1p1;
    motor1Pin2 = m1p2;
    motor2Pin1 = m2p1;
    motor2Pin2 = m2p2;
    pwmMin = 60;
    setup();
}
void Motors::setup()
{
    pinMode(motor1Pin1, OUTPUT);
    pinMode(motor1Pin2, OUTPUT);
    pinMode(motor2Pin1, OUTPUT);
    pinMode(motor2Pin2, OUTPUT);
}
void Motors::forward(int moveTime)
{
    analogWrite(motor1Pin1, pwmMin);
    analogWrite(motor1Pin2, 0);
    // motor leads of motor 2 are reverse
    analogWrite(motor2Pin1, pwmMin);
    analogWrite(motor2Pin2, 0);
    // Using normal delay, interrupts do not use this function
    delay(moveTime);
}
void Motors::moveRight(int moveTime)
{
    analogWrite(motor1Pin1, pwmMin);
    analogWrite(motor1Pin2, 0);
    analogWrite(motor2Pin1, 0);
    analogWrite(motor2Pin2, pwmMin);
    interruptSafeDelay(moveTime);
}
void Motors::moveLeft(int moveTime)
{
    analogWrite(motor1Pin1, 0);
    analogWrite(motor1Pin2, pwmMin);
    analogWrite(motor2Pin1, pwmMin);
    analogWrite(motor2Pin2, 0);
    interruptSafeDelay(moveTime);
}
void Motors::reverse(int moveTime)
{
    analogWrite(motor1Pin1, 0);
    analogWrite(motor1Pin2, pwmMin + 20);
    analogWrite(motor2Pin1, 0);
    analogWrite(motor2Pin2, pwmMin + 20);
    interruptSafeDelay(moveTime);
}
void Motors::stop()
{
    analogWrite(motor1Pin1, 0);
    analogWrite(motor1Pin2, 0);
    analogWrite(motor2Pin1, 0);
    analogWrite(motor2Pin2, 0);
}

void Motors::interruptSafeDelay(int milis)
{
    for (int i = 0; i < milis / 10; i++)
    {
        // The largest value that will produce an accurate microsecond delay is 16383
        // Staying a bit under that
        delayMicroseconds(10000);
    }
}