#include "carActuators.h"
#include "pinout_uno.h"
#include <Servo.h>
#include <ArduinoLog.h>

Servo servoMotor;

void setupActuators()
{
    pinMode(LED_GREEN_PIN, OUTPUT);
    pinMode(LED_YELLOW_PIN, OUTPUT);
    pinMode(LED_RED_PIN, OUTPUT);

    pinMode(DC_MOTOR_ENABLE, OUTPUT);
    pinMode(DC_MOTOR_DIR_A, OUTPUT);
    pinMode(DC_MOTOR_DIR_B, OUTPUT);

    servoMotor.attach(SERVO_PWM_PIN);
}

// set lets depending on given distance
// >100 no leds
// 50-100 green
// 25-50 yellow
// <25 red
void setDistanceLeds(int distance)
{
    if (distance > 100)
    {
        setLeds(LOW, LOW, LOW);
    }
    else if (distance < 100 && distance > 50)
    {
        setLeds(HIGH, LOW, LOW);
    }
    else if (distance < 50 && distance > 25)
    {
        setLeds(HIGH, HIGH, LOW);
    }
    else if (distance < 25)
    {
        setLeds(HIGH, HIGH, HIGH);
    }
}

// set led values
void setLeds(int green, int yellow, int red)
{
    digitalWrite(LED_GREEN_PIN, green);
    digitalWrite(LED_YELLOW_PIN, yellow);
    digitalWrite(LED_RED_PIN, red);
}

// turn dc motor
void setMotorDirection(int16_t motorDirection)
{
    if (abs(motorDirection) < 25)
    {
        digitalWrite(DC_MOTOR_ENABLE, LOW);
        digitalWrite(DC_MOTOR_DIR_A, LOW);
        digitalWrite(DC_MOTOR_DIR_B, LOW);
    }
    else if (motorDirection > 0)
    {
        digitalWrite(DC_MOTOR_ENABLE, HIGH);
        digitalWrite(DC_MOTOR_DIR_A, HIGH);
        digitalWrite(DC_MOTOR_DIR_B, LOW);
    }
    else
    {
        digitalWrite(DC_MOTOR_ENABLE, HIGH);
        digitalWrite(DC_MOTOR_DIR_A, LOW);
        digitalWrite(DC_MOTOR_DIR_B, HIGH);
    }
}

// turn servo to given position
void setServoPosition(int16_t servoPosition)
{
    servoMotor.write(servoPosition);
}
