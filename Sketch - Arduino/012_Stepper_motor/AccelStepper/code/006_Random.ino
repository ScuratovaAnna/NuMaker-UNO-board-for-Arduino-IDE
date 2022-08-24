#include "AccelStepper.h"
AccelStepper stepper; // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5

void setup()
{  
}

void loop()
{
    if (stepper.distanceToGo() == 0)
    {
  // Random change to speed, position and acceleration
  // Make sure we dont get 0 speed or accelerations
  delay(1000);
  stepper.moveTo(rand() % 200);
  stepper.setMaxSpeed((rand() % 200) + 1);
  stepper.setAcceleration((rand() % 200) + 1);
    }
    stepper.run();
}