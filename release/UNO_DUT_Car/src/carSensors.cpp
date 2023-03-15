#include "carSensors.h"
#include "pinout_uno.h"

void setupSensors()
{
    pinMode(USONIC_TRIGGER_PIN, OUTPUT);
    pinMode(USONIC_ECHO_PIN, INPUT);
}

// reads the ultrasonic sensor and calculates the distance in cm
uint32_t getUltrasonicDistance(int triggerPin, int echoPin)
{
    // Set the trigger pin to LOW (2 microseconds)
    digitalWrite(triggerPin, LOW);
    delayMicroseconds(2);

    // Sets the trigger pin to HIGH (10 microseconds)
    digitalWrite(triggerPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(triggerPin, LOW);

    // Reads the echo pin and returns the sound wave travel time
    // in microseconds
    uint32_t duration = pulseIn(echoPin, HIGH, 15000);

    // distance = speed of sound (cm/us) x pulse width = duration * 0.017 || = duration / 58
    uint32_t distance = duration / 58;
    return distance;
}

// read analog input for dc motor and map it
int16_t getMotorDirection(void)
{
    return (analogRead(DC_MOTOR_DIR_IN) - 1023 / 2) / 2;
}

// read analog input for servo and map it
int16_t getServoPosition(void)
{
    return map(analogRead(SERVO_POSITION_IN), 0, 1023, 0, 180);
}