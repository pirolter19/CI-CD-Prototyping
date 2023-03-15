/*
 * File:         test_HC-SR04.cpp
 * Author:       Andreas Fischer
 * Created:      04.01.2023
 */

#include <ArduinoLog.h>
#include "test_HC-SR04.h"
#include "functions_HC-SR04.h"
#include "unity.h"

extern SerialReader serialReader;
extern uint16_t expectedDistance; // cm

void IRAM_ATTR handleInterrupt(void)
{
    delayMicroseconds(100);

    digitalWrite(USONIC_ECHO_PIN_OUT, HIGH);

    uint64_t timestamp = micros();
    uint64_t timestamp_to_low = timestamp + expectedDistance * 58;

    while (micros() < timestamp_to_low)
    {
    }

    digitalWrite(USONIC_ECHO_PIN_OUT, LOW);
}

void setUp(void)
{
    Serial.begin(9600);
    Log.begin(LOG_LEVEL_TRACE, &Serial, false);

    pinMode(USONIC_ECHO_PIN_OUT, OUTPUT); // HC-SR04 Pins
    pinMode(USONIC_TRIGGER_PIN_IN, INPUT);

    pinMode(LED_RED_PIN, INPUT_PULLDOWN); // LED Pins
    pinMode(LED_YELLOW_PIN, INPUT_PULLDOWN);
    pinMode(LED_GREEN_PIN, INPUT_PULLDOWN);

    // Attach an interrupt to TRIGGER_PIN
    attachInterrupt(USONIC_TRIGGER_PIN_IN, handleInterrupt, FALLING);
}

int runUnityTests(void)
{
    UNITY_BEGIN();

    Log.traceln("start tests\n");

    RUN_TEST(test_distance_20);
    RUN_TEST(test_distance_40);
    // RUN_TEST(test_distance_75);
    // RUN_TEST(test_distance_120);

    Log.traceln("\ntests finished");

    delay(5000);
    return UNITY_END();
}

void tearDown(void) // clean stuff up here
{
}

void setup()
{
    // NOTE!!! Wait for >2 secs
    delay(3000);
    setUp();
    delay(2000);
    runUnityTests();
    delay(3000); // NOTE!!! Wait for >2 secs
}

void loop()
{
}
