#ifndef CAR_ACTUATORS_h
#define CAR_ACTUATORS_h

#include <Arduino.h>

void setupActuators();
void setLeds(int green, int yellow, int red);
void setDistanceLeds(int distance);
void setMotorDirection(int16_t motorDirection);
void setServoPosition(int16_t servoPosition);

#endif // CAR_ACTUATORS_h