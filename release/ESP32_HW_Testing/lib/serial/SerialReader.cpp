#include "SerialReader.h"
#include <ArduinoLog.h>
#include "serial_tags.h"

/// @brief initializes a new serial connection for the given baudrate, rxPin and txPin
/// @param rxPin pin for receiving data
/// @param txPin pin for transmitting data
/// @param baudRate baudrate for the connection
SerialReader::SerialReader(uint8_t rxPin, uint8_t txPin, long baudRate)
{
    // Set up a new SoftwareSerial object
    _mySerial.begin(baudRate, SWSERIAL_8N1, rxPin, txPin, false);

    // Configure pin direction
    pinMode(rxPin, INPUT);
    pinMode(txPin, OUTPUT);
}

/// @brief reads data from _mySerial and writes it into the given buffer
/// @param buffer buffer to store read data from serial
void SerialReader::read(char *buffer)
{
    uint8_t cnt = 0;
    uint8_t tmp;

    // While the serial connection is available
    // the data gets stored into the provided buffer
    while (_mySerial.available())
    {
        tmp = (uint8_t)_mySerial.read();
        // Log.verboseln("tmp: %d", tmp);
        buffer[cnt] = (char)tmp;
        cnt++;
    }

    buffer[cnt] = END_CHAR; // Add NULL byte to string
}

/// @brief waits for the given prefix (%c: e.g. d: for distance) and stores all data after prefix until '\n', max buffer size or timeout is reached.
/// @param prefix prefix to wait for, before storing data in buffer
char *SerialReader::read(char prefix)
{
    memset(&(_serialReaderBuffer[0]), END_CHAR, SERIAL_READER_BUFFER_SIZE);

    uint8_t cnt = 0;
    char tmp;
    bool rdyToRead = false;
    bool prefixFound = false;
    uint64_t timeout = millis() + READ_TIMEOUT;

    while (millis() < timeout && tmp != NEW_LINE_CHAR)
    {
        while (_mySerial.available())
        {
            tmp = (char)_mySerial.read();
            Log.verboseln("tmp: %c", tmp);
            if (tmp == NEW_LINE_CHAR || cnt == SERIAL_READER_BUFFER_SIZE - 1)
            {
                return _serialReaderBuffer;
            }
            if (rdyToRead && prefixFound)
            {
                _serialReaderBuffer[cnt] = tmp;
                cnt++;
                Log.verboseln("read value");
            }
            else if (prefixFound)
            {
                // remove ':'
                tmp = (char)_mySerial.read();
                rdyToRead = true;
                Log.verboseln("rdyToRead");
            }
            else if (tmp == prefix)
            {
                prefixFound = true;
                Log.verboseln("prefixFound");
            }
        }
    }

    return _serialReaderBuffer;
}

/// @brief waits for the given prefix (%c: e.g. d: for distance) and stores all data after prefix until '\n', max buffer size or timeout is reached.
/// @param prefix prefix to wait for, before storing data in buffer
array<char, SERIAL_READER_BUFFER_SIZE> SerialReader::readPrefix(char prefix)
{
    _buffer.fill(END_CHAR);

    uint8_t cnt = 0;
    char tmp;
    bool rdyToRead = false;
    bool prefixFound = false;
    uint64_t timeout = millis() + READ_TIMEOUT;

    while (millis() < timeout && tmp != NEW_LINE_CHAR)
    {
        while (_mySerial.available())
        {
            tmp = (char)_mySerial.read();
            Log.verboseln("tmp: %c", tmp);
            if (rdyToRead && prefixFound)
            {
                _buffer.at(cnt) = tmp;
                cnt++;
                Log.verboseln("_serialReaderBuffer");
            }
            else if (prefixFound)
            {
                // remove ':'
                tmp = (char)_mySerial.read();
                rdyToRead = true;
                Log.verboseln("rdyToRead");
            }
            else if (tmp == prefix)
            {
                prefixFound = true;
                Log.verboseln("prefixFound");
            }
        }
    }

    return _buffer;
}

// char *SerialReader::read(char prefix)
// {
//     memset(&(_serialReaderBuffer[0]), END_CHAR, SERIAL_READER_BUFFER_SIZE);

//     uint8_t cnt = 0;
//     char tmp;
//     bool rdyToRead = false;
//     uint64_t timeout = millis() + READ_TIMEOUT;

//     while (_mySerial.available())
//     {
//         char preCheck[2];
//         _mySerial.readBytes(preCheck, 2);

//         Log.verboseln("preTmp: %c", preCheck[0]);
//         Log.verboseln("preColon: %c", preCheck[1]);

//         if (preCheck[0] == prefix && preCheck[1] == ':' && !rdyToRead)
//         {
//             Log.verboseln("rdyToRead");

//             rdyToRead = true;
//         }
//     }

//     Log.verboseln("rdyToRead while");

//     while (rdyToRead && cnt < SERIAL_READER_BUFFER_SIZE && millis() < timeout)
//     {
//         while (_mySerial.available())
//         {
//             tmp = (char)_mySerial.read();
//             Log.verboseln("tmp: %c", tmp);

//             if (tmp == NEW_LINE_CHAR)
//             {
//                 return _serialReaderBuffer;
//             }
//             _serialReaderBuffer[cnt] = tmp;
//             cnt++;
//         }
//     }

//     Log.verboseln("serialReaderBuffer: %s", _serialReaderBuffer);
//     return _serialReaderBuffer;
// }

// /// @brief reads data from _mySerial and waits for the given prefix, writes following data into buffer
// /// @param buffer buffer to store read data from serial
// /// @param prefix prefix to wait for, before storing data in buffer
// array<char, BUFFER_SIZE> SerialReader::read(char prefix)
// {
//     uint8_t cnt = 0;
//     char tmp;
//     bool rdyToRead;

//     // reset buffer
//     _serialReaderBuffer.fill(END_CHAR);

//     while (_mySerial.available())
//     {
//         tmp = (char)_mySerial.read();
//         Log.verboseln("tmp: %c", tmp);
//         if (tmp == prefix && !rdyToRead)
//         {
//             rdyToRead = true;
//         }
//         else if (tmp == NEW_LINE_CHAR)
//         {
//             break;
//         }
//         else if (rdyToRead)
//         {
//             _serialReaderBuffer[cnt] = tmp;
//             cnt++;
//         }
//     }

//     _serialReaderBuffer[cnt] = END_CHAR; // Add NULL byte to string
//     Log.verboseln("_serialReaderBuffer: %d", atoi(_serialReaderBuffer.data()));

//     return _serialReaderBuffer;
// }
