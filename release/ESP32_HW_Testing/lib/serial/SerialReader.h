#ifndef SERIAL_READER
#define SERIAL_READER

#include "Arduino.h"
#include <SoftwareSerial.h>
#include <array>

using namespace std;

#define SERIAL_READER_BUFFER_SIZE 64
#define READ_TIMEOUT 2000

class SerialReader
{
public:
  SerialReader(uint8_t rxPin, uint8_t txPin, long baudRate);
  void read(char *buffer);
  char *read(char prefix);
  array<char, SERIAL_READER_BUFFER_SIZE> readPrefix(char prefix);

private:
  SoftwareSerial _mySerial;
  char _serialReaderBuffer[SERIAL_READER_BUFFER_SIZE];
  array<char, SERIAL_READER_BUFFER_SIZE> _buffer;
};

#endif // SERIAL_READER
