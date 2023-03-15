/*
 * File:         test.cpp
 * Author:       Andreas Fischer
 * Created:      12.01.2023
 */

#include <Arduino.h>
#include <ArduinoLog.h>
#include <unity.h>
#include "carActuators.cpp"
#include "pinout_uno.h"

void test_setDistanceLeds(void);
void test_setLeds(void);
void readLeds_distance(uint8_t distance);
void readLeds(bool green, bool yellow, bool red);

void readLeds(bool green, bool yellow, bool red)
{
    Log.trace("\033[0m\ttest_setLeds\t Green = %d,\t yellow = %d,\t red = %d\t ", green, yellow, red);

    TEST_ASSERT_EQUAL(green, digitalRead(LED_GREEN_PIN));
    TEST_ASSERT_EQUAL(yellow, digitalRead(LED_YELLOW_PIN));
    TEST_ASSERT_EQUAL(red, digitalRead(LED_RED_PIN));

    Log.traceln("\033[1;32mPASSED!");
    Log.trace("\033[0m");
}

void test_setLeds(void)
{
    Log.traceln("\n\n\033[0mtest_setLeds:");

    setLeds(0, 0, 0);
    readLeds(0, 0, 0);

    setLeds(1, 0, 0);
    readLeds(1, 0, 0);

    setLeds(0, 1, 0);
    readLeds(0, 1, 0);

    setLeds(0, 0, 1);
    readLeds(0, 0, 1);

    setLeds(1, 1, 1);
    readLeds(1, 1, 1);

    Log.traceln("\033[0m\ttest_setLeds finished \n");
}

void test_setDistanceLeds(void)
{
    Log.traceln("test_setDistanceLeds:");

    for (int i = 10; i < 150; i += 5)
    {
        setDistanceLeds(i);
        delay(1);
        readLeds_distance(i);

        Log.trace("\033[0m\ttest_setDistanceLeds %d cm:\t", i);
        Log.traceln("\033[1;32mPASSED!");
        Log.trace("\033[0m");
    }

    Log.traceln("\033[0m\ttest_setDistanceLeds finished \n");
}

void readLeds_distance(uint8_t distance)
{
    if (distance > 100)
    {
        TEST_ASSERT_EQUAL(LOW, digitalRead(LED_GREEN_PIN));
        TEST_ASSERT_EQUAL(LOW, digitalRead(LED_YELLOW_PIN));
        TEST_ASSERT_EQUAL(LOW, digitalRead(LED_RED_PIN));
    }
    else if (distance < 100 && distance > 50)
    {
        TEST_ASSERT_EQUAL(HIGH, digitalRead(LED_GREEN_PIN));
        TEST_ASSERT_EQUAL(LOW, digitalRead(LED_YELLOW_PIN));
        TEST_ASSERT_EQUAL(LOW, digitalRead(LED_RED_PIN));
    }
    else if (distance < 50 && distance > 25)
    {
        TEST_ASSERT_EQUAL(HIGH, digitalRead(LED_GREEN_PIN));
        TEST_ASSERT_EQUAL(HIGH, digitalRead(LED_YELLOW_PIN));
        TEST_ASSERT_EQUAL(LOW, digitalRead(LED_RED_PIN));
    }
    else if (distance < 25)
    {
        TEST_ASSERT_EQUAL(HIGH, digitalRead(LED_GREEN_PIN));
        TEST_ASSERT_EQUAL(HIGH, digitalRead(LED_YELLOW_PIN));
        TEST_ASSERT_EQUAL(HIGH, digitalRead(LED_RED_PIN));
    }
}

void tearDown(void)
{
    // clean stuff up here
    Serial.end();
}

int runUnityTests(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_setLeds);
    RUN_TEST(test_setDistanceLeds);
    delay(5000);
    return UNITY_END();
}

void setup()
{
    delay(3000); // NOTE!!! Wait for >2 secs

    Serial.begin(9600);
    Log.begin(LOG_LEVEL_TRACE, &Serial, false);

    setupActuators();

    Log.traceln("*** setup finished ***");

    delay(2000);

    runUnityTests();
}

void loop()
{
}
