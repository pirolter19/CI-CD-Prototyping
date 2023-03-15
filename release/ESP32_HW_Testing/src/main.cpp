#include <Arduino.h>
#include <ArduinoLog.h>
#include "SerialReader.h"
#include "driver/dac.h"
#include "serial_tags.h"

SerialReader serialReader(10, 11, 9600);

void setupSerialReader()
{
    Serial.begin(9600);
    Log.begin(LOG_LEVEL_TRACE, &Serial, false);

    Log.traceln("*** Serial started ***\n");
    // pinMode(17, OUTPUT);

    // dacWrite(17, 100);
    delay(100);
}

void loopSerialReader()
{
    auto data = serialReader.read(USONIC_DISTANCE_SERIAL_TAG);
    Log.traceln("data: %d", atoi(data));

    // auto data1 = serialReader.read(MOTOR_SERIAL_TAG);
    // Log.traceln("data1: %d", atoi(data1));

    // auto dataArr = serialReader.readPrefix(USONIC_DISTANCE_SERIAL_TAG);
    // Log.traceln("dataArr: %d", atoi(dataArr.data()));

    // delay(40);

    // auto dataArr1 = serialReader.readPrefix(MOTOR_SERIAL_TAG);
    // Log.traceln("dataArr1: %d", atoi(dataArr1.data()));
    // dacWrite(17, 100);

    // auto actualServoValue = serialReader.read(SERVO_SERIAL_TAG);
    // Log.traceln("actualServoValue: %s", actualServoValue);
}

void loopDacTest()
{
    // dac_output_enable(DAC_CHANNEL_1);

    for (size_t i = 0; i < 255; i++)
    {
        dacWrite(17, i);
        dacWrite(18, i);
        delay(20);
    }
}

void setup()
{
    // for testing only

    // setupDacAdc();
    // setupHcSr04();
    // setupFanMotor();
    // setupFreeRtos();
    // setupSerialReader();
    // setupDacWrite();

    // pinMode(17, OUTPUT);
    // pinMode(18, OUTPUT);
    delay(100);
}

void loop()
{
    // loopDacAdc();
    // loopHcSr04();
    // loopFanMotor();
    // loopFreeRtos();
    // loopSerialReader();
    // loopDacWrite();
    // loopDacTest();
}