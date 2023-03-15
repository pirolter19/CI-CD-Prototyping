#include <Arduino.h>
#include <ArduinoLog.h>
#include <unity.h>
#include "test_HC-SR04.h"
#include "timeout_handler.h"

extern SerialReader serialReader;
extern uint16_t expectedDistance; // cm

int getMeanValueOfReadValues(uint8_t samples)
{
    int sum = 0;
    for (int i = 0; i < samples;)
    {
        auto readValue = serialReader.read(USONIC_DISTANCE_SERIAL_TAG);
        Log.verboseln("readValue = %s", readValue);
        int value = atoi(readValue);

        // if value is not empty
        if (value > 0)
        {
            sum += value;
            i++;
        }
    }

    return sum / samples;
}

void test_distance(uint16_t distance, bool expectedRedLed, bool expectedYellowLed, bool expectedGreenLed)
{
    delay(delay_time);
    expectedDistance = distance;
    int readDistance = getMeanValueOfReadValues(10);

    Log.traceln("distance:\texpected=%d,\t actual=%d", expectedDistance, readDistance);

    bool LED[3] = {digitalRead(LED_RED_PIN),    // actual LED status: RED, YELLOW, GREEN
                   digitalRead(LED_YELLOW_PIN), //
                   digitalRead(LED_GREEN_PIN)}; //

    Log.traceln("LED red:\texpected=%d,\t actual=%d", expectedRedLed, LED[0]);
    Log.traceln("LED yellow:\texpected=%d,\t actual=%d", expectedYellowLed, LED[1]);
    Log.traceln("LED green:\texpected=%d,\t actual=%d", expectedGreenLed, LED[2]);

    TEST_ASSERT_INT_WITHIN(15, expectedDistance, readDistance);

    TEST_ASSERT_EQUAL(expectedRedLed, LED[0]);
    TEST_ASSERT_EQUAL(expectedYellowLed, LED[1]);
    TEST_ASSERT_EQUAL(expectedGreenLed, LED[2]);
}

void test_distance_20(void)
{
    test_distance(20, true, true, true);
}

void test_distance_40(void)
{
    test_distance(40, false, true, true);
}

void test_distance_75(void)
{
    test_distance(75, false, false, true);
}

void test_distance_120(void)
{
    test_distance(120, false, false, false);
}