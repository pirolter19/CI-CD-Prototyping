#ifndef CAR_SENSORS_h
#define CAR_SENSORS_h

#include <Arduino.h>

void setupSensors();
uint32_t getUltrasonicDistance(int triggerPin, int echoPin);
int16_t getMotorDirection(void);
int16_t getServoPosition(void);

#endif // CAR_SENSORS_h