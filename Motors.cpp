
class Motors
{
public:
    int motor1Pin1;
    int motor1Pin2;
    int motor2Pin1;
    int motor2Pin2;
    int pwmMin = 60;

    Motors(int m1p1, int m1p2, int m2p1, int m2p2)
    {
        motor1Pin1 = m1p1;
        motor1Pin2 = m1p2;
        motor2Pin1 = m2p1;
        motor2Pin2 = m2p2;
        setup();
    }

    void setup()
    {
        pinMode(motor1Pin1, OUTPUT);
        pinMode(motor1Pin2, OUTPUT);
        pinMode(motor2Pin1, OUTPUT);
        pinMode(motor2Pin2, OUTPUT);
    }

    void forward(int moveTime)
    {
        analogWrite(motor1Pin1, pwmMin);
        analogWrite(motor1Pin2, 0); //PWM value where 0 = 0% and 255 = 100%
        // motor leads of motor 2 are reverse
        analogWrite(motor2Pin1, pwmMin);
        analogWrite(motor2Pin2, 0);
        delay(moveTime);
    }
    void moveRight(int moveTime)
    {
        analogWrite(motor1Pin1, 0);
        analogWrite(motor1Pin2, pwmMin);
        analogWrite(motor2Pin1, pwmMin);
        analogWrite(motor2Pin2, 0);

        delay(moveTime);
    }

    void moveLeft(int moveTime)
    {
        analogWrite(motor1Pin1, pwmMin);
        analogWrite(motor1Pin2, 0);
        analogWrite(motor2Pin1, 0);
        analogWrite(motor2Pin2, pwmMin + 20);
        delay(moveTime);
    }

    void reverse(int moveTime)
    {
        analogWrite(motor1Pin1, 0);
        analogWrite(motor1Pin2, pwmMin + 20);
        analogWrite(motor2Pin1, 0);
        analogWrite(motor2Pin2, pwmMin + 20);
        delay(moveTime);
    }

    void stop()
    {
        analogWrite(motor1Pin1, 0);
        analogWrite(motor1Pin2, 0);
        analogWrite(motor2Pin1, 0);
        analogWrite(motor2Pin2, 0);
    }
};
