/*
 * File:         fan_motor_tests.cpp
 * Author:       Erik Pirolt
 * Created:      11.12.2022
 */

#include <Arduino.h>
#include <ArduinoLog.h>
#include <unity.h>
#include <PwmReader.cpp>
#include <driver/dac.h>
#include "pinout_esp.h"
#include "timeout_handler.h"

#define TimeOut 10000

void setUp(void)
{
    Serial.begin(9600);
    initPwmReader();
    // Initialize with log level and log output.
    Log.begin(LOG_LEVEL_VERBOSE, &Serial, true);
    pinMode(DC_MOTOR_ANALOG_DIR_OUT, OUTPUT);
}

void tearDown(void)
{
    // clean stuff up here
}

void test_checkDutyCycle_20(void)
{
    int expected_value = 20;
    Log.traceln("test_checkDutyCycle expected: %d", expected_value);

    TimeoutHandler timeoutHandler(TimeOut);
    timeoutHandler.start();

    while (!readPwmData(1) && !timeoutHandler.hasTimeout())
    {
    }

    readPwmData(1);

    int actualValue = getPwmDutyCycle();

    Log.traceln("actual: %d, exp: %d", actualValue, expected_value);
    Log.traceln("Hightime:%f , Timewidth:%f", getPulseWidthLow(), getPwmPeriodTime());
    TEST_ASSERT_INT_WITHIN(4, expected_value, actualValue);
}

int runUnityTests(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_checkDutyCycle_20);
    delay(5000);
    return UNITY_END();
}

void setup()
{
    // NOTE!!! Wait for >2 secs
    delay(5000);
    setUp();
    dac_output_enable(DAC_CHANNEL_1);
    dac_output_voltage(DAC_CHANNEL_1, 50);
    delay(2000);
    runUnityTests();
}

void loop()
{
}
