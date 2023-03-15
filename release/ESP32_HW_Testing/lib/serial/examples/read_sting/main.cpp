#include <Arduino.h>
#include "SerialReader.h"

#define rxPin 10
#define txPin 11
#define buadRate 9600

char buffer[64];

// Set up a new SoftwareSerial object
SerialReader serialReader(rxPin, txPin, baudRate);

void setup()
{
  Serial.begin(baudRate);
  Serial.println("*** started ***");
}

void loop()
{
  serialReader.read(buffer);

  Serial.print(buffer);
}

// COM6