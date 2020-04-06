
class Motors
{
public:
  int motor1Pin1;
  int motor1Pin2;
  int motor2Pin1;
  int motor2Pin2;
  int pwmMin;

  Motors(int m1p1, int m1p2, int m2p1, int m2p2);
  void setup();
  void forward(int moveTime);
  void moveRight(int moveTime);
  void moveLeft(int moveTime);
  void reverse(int moveTime);
  void stop();
};
