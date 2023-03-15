#include <Arduino.h>
#include <ArduinoLog.h>
#include <unity.h>
#include "timeout_handler.h"

void hasTimeout_after100ms()
{
    delay(300);
    uint64_t expectedTimeout = 100;
    TimeoutHandler timeoutHandler(expectedTimeout);
    timeoutHandler.start();

    TEST_ASSERT_FALSE(timeoutHandler.hasTimeout());
    delay(100);
    TEST_ASSERT_TRUE(timeoutHandler.hasTimeout());
}

void hasTimeout_runWhile_300ms()
{
    uint64_t expectedTimeout = 300;
    TimeoutHandler timeoutHandler(expectedTimeout);
    timeoutHandler.start();
    uint64_t time = millis();

    while (!timeoutHandler.hasTimeout())
    {
        delay(10);
    }

    uint64_t runTime = millis() - time;
    TEST_ASSERT_TRUE(timeoutHandler.hasTimeout());
    TEST_ASSERT_UINT_WITHIN(10, expectedTimeout, runTime);
}

void setUp(void)
{
    Serial.begin(9600);
    Log.begin(LOG_LEVEL_TRACE, &Serial, false);
}

void tearDown(void)
{
}

int runUnityTests(void)
{
    UNITY_BEGIN();
    RUN_TEST(hasTimeout_after100ms);
    RUN_TEST(hasTimeout_runWhile_300ms);
    return UNITY_END();
}

void setup()
{
    setUp();
    runUnityTests();
}

void loop()
{
}
