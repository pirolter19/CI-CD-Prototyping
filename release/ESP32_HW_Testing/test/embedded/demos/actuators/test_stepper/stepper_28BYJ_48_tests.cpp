/*
 * File:         stepper_28BYJ_48_tests.cpp
 * Author:       Philipp Steinwender
 * Created:      05.12.2022
 */

#include <Arduino.h>
#include <ArduinoLog.h>
#include <unity.h>
#include <PwmReader.cpp>

#define StepperIn1 18 // pin mapped from esp to stepper out8 on arduino
#define StepperIn2 19 // pin mapped from esp to stepper out9 on arduino
#define StepperIn3 20 // pin mapped from esp to stepper out10 on arduino
#define StepperIn4 21 // pin mapped from esp to stepper out11 on arduino
#define SpeedOut 17   // pin for setting speed on dut 8bit (0-255)

void setUp(void)
{
    Serial.begin(9600);

    // Initialize with log level and log output.
    Log.begin(LOG_LEVEL_TRACE, &Serial, true);

    // pinMode(SpeedOut, OUTPUT);

    // init pwm reader for pins 18, 19, 20, 21
    initPwmReader();

    Log.traceln("setup tests");
}

void tearDown(void)
{
    // clean stuff up here
}

void test_checkSpeed_SetTo250Analog_expected250ms(void)
{
    Log.traceln("test_checkSpeed_SetTo150Analog_expected150ms");
    float expectedSpeedMs = 250.0;
    uint16_t mappedSpeed = map(expectedSpeedMs, 0, 500, 0, 255);    // map speed to 8-bit dac range (0-255)
    Log.traceln("mappedSpeed = %d", mappedSpeed);
    dacWrite(SpeedOut, mappedSpeed);
    
    uint32_t timeout = micros() + 7500;
    uint8_t channelToRead = 1;

    while (!readPwmData(channelToRead) && micros() <= timeout)
    {
    }

    for (channelToRead = 1; channelToRead < 5; channelToRead++)
    {
        delay(1500);
        readPwmData(channelToRead);

        Log.traceln("ch: %d, actual: %F, exp: %F", channelToRead, getPulseWidthHigh(), expectedSpeedMs);
        TEST_ASSERT_FLOAT_WITHIN(0.5, expectedSpeedMs, getPulseWidthHigh());
    }
}

void test_checkSpeed_SetTo150_expected150ms(void)
{
    Log.traceln("test_checkSpeed_SetTo150_expected150ms");
    float expectedSpeedMs = 150.0;
    uint32_t timeout = micros() + 7500;
    uint8_t channelToRead = 1;

    while (!readPwmData(channelToRead) && micros() <= timeout)
    {
    }

    for (channelToRead = 1; channelToRead < 5; channelToRead++)
    {
        delay(1500);
        readPwmData(channelToRead);

        Log.traceln("ch: %d, actual: %F, exp: %F", channelToRead, getPulseWidthHigh(), expectedSpeedMs);
        TEST_ASSERT_FLOAT_WITHIN(0.5, expectedSpeedMs, getPulseWidthHigh());
    }
}

int runUnityTests(void)
{
    UNITY_BEGIN();
    // RUN_TEST(test_checkSpeed_SetTo150_expected150ms);
    // delay(2000);
    // RUN_TEST(test_checkSpeed_SetTo250Analog_expected250ms);
    // delay(2000);
    return UNITY_END();
}

void setup()
{
    // NOTE!!! Wait for >2 secs
    delay(2000);
    setUp();
    runUnityTests();
}

void loop()
{
}
