#include <Arduino.h>
#include <ArduinoLog.h>

#define DAC0 17 // DAC-channel 0
#define ADC_IN 2

void setupDacAdc()
{
  Serial.begin(9600);

  // Initialize with log level and log output.
  Log.begin(LOG_LEVEL_TRACE, &Serial, true);

  Log.traceln("setup");

  // set the resolution to 12 bits (0-4096)
  analogReadResolution(12);
}

void loopDacAdc()
{
  // 255= 3.3V
  uint16_t mappedSpeed = map(500, 0, 500, 0, 255);
  Log.traceln("mappedSpeed = %d", mappedSpeed);
  dacWrite(DAC0, mappedSpeed);
  delay(500);

  // 4095 = 3.3V
  int analogIn = analogRead(ADC_IN);
  uint16_t mappedAnalogIn = map(analogIn, 0, 4095, 0, 500);
  Log.traceln("mappedAnalogIn = %d", mappedAnalogIn);
}