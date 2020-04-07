#ifndef ACTUATOR_CLASS
#define ACTUATOR_CLASS

class Actuator
{
public:
  virtual void setup() = 0;
  virtual void interruptSafeDelay(int milis) = 0;
};

#endif