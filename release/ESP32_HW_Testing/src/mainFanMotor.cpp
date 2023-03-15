#include <Arduino.h>
#include <ArduinoLog.h>
#include <PwmReader.cpp>
#include <driver/dac.h>

#define PWMIn 3
#define AnaOut 17

void setupFanMotor()
{
  // put your setup code here, to run once:
  Serial.begin(9600);

  // Initialize with log level and log output.
  Log.begin(LOG_LEVEL_TRACE, &Serial, true);

  // Start logging text and formatted values
  Log.noticeln("starting stepper project.");

  initPwmReader();

  // ledcSetup(0, 490, 8);
  // ledcAttachPin(PWM_Output, 0); // ledcAttaachPin(Pin-number, Channel-number
  pinMode(AnaOut, OUTPUT);
  pinMode(PWMIn, INPUT_PULLUP);
}

void loopFanMotor()
{
  // put your main code here, to run repeatedly:
  uint8_t i = 1;

  dac_output_enable(DAC_CHANNEL_1);
  dac_output_voltage(DAC_CHANNEL_1, 50);

  Log.traceln("\n ####### channel = %d", i);
  // if a new pulse is detected on channel 1, print the pulse width to serial monitor.
  if (readPwmData(i))
  {
    float pulseWidth = getPulseWidthLow();
    Log.traceln("pulseWidth = %F", pulseWidth);

    long pulseTime = getPwmPulseStartTime();
    Log.traceln("pulseTime = %l", pulseTime);

    float period = getPwmPeriodTime();
    Log.traceln("period = %F", period);

    float highTime = period - pulseWidth;
    Log.noticeln("highTime = %F", highTime);
  }
  delay(1000);
}