/**
* https://www.arduino.cc/reference/en/libraries/accelstepper/
* https://www.airspayce.com/mikem/arduino/AccelStepper/index.html
*/
#include "AccelStepper.h"

// Define a stepper and the pins it will use
AccelStepper stepper; // Defaults to AccelStepper::FULL4WIRE (4 pins) on 2, 3, 4, 5

void setup()
{  
  // Change these to suit your stepper if you want
  stepper.setMaxSpeed(800);
  stepper.setAcceleration(20);
  stepper.moveTo(800);
}

void loop()
{
    // If at the end of travel go to the other end
    if (stepper.distanceToGo() == 0)
      stepper.moveTo(-stepper.currentPosition());

    stepper.run();
}
