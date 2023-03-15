/*
 * File:         readWrite_freeRtos_test.cpp
 * Author:       Philipp Steinwender
 * Created:      20.12.2022
 */

#include <Arduino.h>
#include <ArduinoLog.h>
#include <unity.h>
#include <FreeRTOS.h>

#define DAC0 17           // DAC-channel 0
#define ADC_IN 1          // ADC1_0
#define DAC_MAP_VALUE 208 // map to 0-208 because we cant use full 3.3V -> adc can't handle it

static TaskHandle_t Task1;
static TaskHandle_t Task2;
uint32_t valueToWrite = 100;
uint32_t mappedAnalogIn = 0;

void writeValue(void *parameter)
{
    Log.traceln("Task1 is running on core %d", xPortGetCoreID());
    vTaskDelay(20 / portTICK_PERIOD_MS);

    while (true)
    {
        uint16_t mappedSpeed = map(valueToWrite, 0, 500, 0, DAC_MAP_VALUE);
        // Log.traceln("valueToWrite = %d", valueToWrite);
        dacWrite(DAC0, mappedSpeed);
        vTaskDelay(20 / portTICK_PERIOD_MS);
        valueToWrite++;
        if (valueToWrite >= 500)
        {
            vTaskSuspend(Task2);
            vTaskSuspend(Task1);
        }
    }
}

void readValue(void *parameter)
{
    // vTaskDelay(20 / portTICK_PERIOD_MS);

    while (true)
    {
        uint16_t analogIn = analogRead(ADC_IN);
        mappedAnalogIn = map(analogIn, 0, 4095, 0, 500);
        // Log.traceln("mappedAnalogIn = %d", mappedAnalogIn);
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void setUp(void)
{
    Serial.begin(9600);

    // Initialize with log level and log output.
    Log.begin(LOG_LEVEL_TRACE, &Serial, true);
    pinMode(DAC0, OUTPUT);

    // set the resolution to 12 bits (0-4096)
    analogReadResolution(12);

    // 0 mV ~ 2500 mV adc range
    analogSetAttenuation(ADC_11db);

    Log.traceln("setup tests");
}

void tearDown(void)
{
    // clean stuff up here
}

void read_write_values_parallel()
{
    valueToWrite = 100;

    Log.noticeln("read_write_values_parallel");

    xTaskCreatePinnedToCore(readValue, "readValueTask", 10000, NULL, 1, &Task2, 1);
    delay(20);

    xTaskCreatePinnedToCore(writeValue, "writeValueTask", 10000, NULL, 1, &Task1, 0);
    delay(20);

    while (!(valueToWrite >= 500) && !(mappedAnalogIn >= 500))
    {
        delay(50);
        Log.traceln("valueToWrite = %d, mappedAnalogIn = %d, delta = %d", valueToWrite, mappedAnalogIn, (valueToWrite - mappedAnalogIn));
        TEST_ASSERT_UINT32_WITHIN(20, valueToWrite, mappedAnalogIn);
    }
}

int runUnityTests(void)
{
    UNITY_BEGIN();
    RUN_TEST(read_write_values_parallel);
    delay(2000);
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
