/*
 * File:         servo_motor_tests.cpp
 * Author:       Philipp Steinwender
 * Created:      11.01.2023
 */

#include <Arduino.h>
#include <ArduinoLog.h>
#include <unity.h>
#include "SerialReader.h"
#include "timeout_handler.h"
#include "pinout_esp.h"
#include "serial_tags.h"

#define TimeOut 200000    // timeout for tests
#define DAC_MAP_VALUE 280 // 255 ~ 3.3V

SerialReader serialReader(10, 11, 9600); // serial reader for checking printed values on uno

int getMeanValueOfReadValues(uint8_t samples)
{
    int sum = 0;
    for (int i = 0; i < samples;)
    {
        int readValue = atoi(serialReader.read(SERVO_SERIAL_TAG));
        Log.verboseln("readValue = %d", readValue);
        if (readValue > 0)
        {
            sum += readValue;
            i++;
        }
    }

    return sum / samples;
}

// 0 - 180° servo position value can be set in tests and will be written via dac
void writeDacValue(int servoPosition)
{
    // todo check what range we really have, dac ~ 0.2-3 V
    uint16_t mappedValueToWrite = map(servoPosition, 0, 180, 0, DAC_MAP_VALUE);
    Log.verboseln("mappedValueToWrite = %d", mappedValueToWrite);
    dacWrite(SERVO_ANALOG_POSITION_OUT, mappedValueToWrite);
}

void set_servo_position_100()
{
    uint8_t expectedServoPosition = 100;
    Log.traceln("running set_servo_position_100");

    delay(100);
    writeDacValue(expectedServoPosition);

    auto actualServoValue = getMeanValueOfReadValues(10);

    Log.traceln("delta=10\texpected=%d\tactual=%d", expectedServoPosition, actualServoValue);
    TEST_ASSERT_UINT32_WITHIN(10, expectedServoPosition, actualServoValue);
}

void set_servo_position_10_170()
{
    Log.traceln("running set_servo_position_10_170");

    for (int rotationToWrite = 10; rotationToWrite < 150;)
    {
        writeDacValue(rotationToWrite);

        auto actualServoValue = getMeanValueOfReadValues(3);

        Log.traceln("delta=10\texpected=%d\tactual=%d", rotationToWrite, actualServoValue);
        TEST_ASSERT_UINT32_WITHIN(10, rotationToWrite, actualServoValue);

        rotationToWrite += 10;
    }
}

void setUp(void)
{
    Serial.begin(9600);
    Log.begin(LOG_LEVEL_TRACE, &Serial, false);
    pinMode(SERVO_ANALOG_POSITION_OUT, OUTPUT);
}

void tearDown(void)
{
    // clean stuff up here
}

int runUnityTests(void)
{
    UNITY_BEGIN();
    TimeoutHandler timeoutHandler(TimeOut);
    timeoutHandler.start();

    while (!timeoutHandler.hasTimeout())
    {
        RUN_TEST(set_servo_position_100);
        RUN_TEST(set_servo_position_10_170);
        break;
    }

    delay(5000);
    return UNITY_END();
}

void setup()
{
    // NOTE!!! Wait for >2 secs
    delay(3000);
    setUp();
    delay(2000);
    runUnityTests();
}

void loop()
{
}
