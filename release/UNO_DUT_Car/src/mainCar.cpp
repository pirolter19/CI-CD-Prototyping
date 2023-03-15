#include <Arduino.h>
#include <ArduinoLog.h>
#include "carSensors.h"
#include "carActuators.h"
#include "pinout_uno.h"
#include <SoftwareSerial.h>
#include <SoftwareSerial.h>

// Parameters
#define CYCLE_TIME 100
uint64_t timestamp = 0;

// Set up a new SoftwareSerial object
SoftwareSerial mySerial(RX_PIN, TX_PIN);

void setup()
{
  // Serial.begin(9600);

  // Define pin modes for TX and RX
  pinMode(RX_PIN, INPUT);
  pinMode(TX_PIN, OUTPUT);
  mySerial.begin(9600);

  // Initialize with log level and log output.
  Log.begin(LOG_LEVEL_VERBOSE, &mySerial, false);

  // Start logging text and formatted values
  Log.noticeln("starting car, brumm bumm....");

  // setup ultrasonic sensor
  setupSensors();

  // setup servo, dc motor and leds
  setupActuators();
}

void loop()
{
  // Cycle Time
  while (millis() < timestamp + CYCLE_TIME)
  {
  };

  // create new timestamp for cycle time
  timestamp = millis();

  // read/calculate distance from ultrasonic sensor
  uint32_t distance = getUltrasonicDistance(USONIC_TRIGGER_PIN, USONIC_ECHO_PIN);
  Log.verboseln("d:%d", distance);

  // read values for servo and dc motor
  int16_t servoPosition = getServoPosition();
  Log.verboseln("s:%d", servoPosition);

  int16_t motorDirection = getMotorDirection();
  Log.verboseln("m:%d", motorDirection);

  // set leds depending on distance
  setDistanceLeds(distance);

  // turn servo to given position
  setServoPosition(servoPosition);

  // turn dc motor
  setMotorDirection(motorDirection);
}
