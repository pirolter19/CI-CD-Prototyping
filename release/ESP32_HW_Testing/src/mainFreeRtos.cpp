#include <Arduino.h>
#include <FreeRTOS.h>
#include <ArduinoLog.h>
#include "esp32-hal-adc.h"

#define DAC0 17           // DAC-channel 0
#define ADC_IN 1          // ADC1_0
#define DAC_MAP_VALUE 208 // map to 0-208 because we cant use full 3.3V -> adc can't handle it

TaskHandle_t Task1;
TaskHandle_t Task2;
int readValue = 0;

void Task1code(void *parameter)
{
    Log.traceln("Task1 is running on core %d", xPortGetCoreID());
    uint16_t valueToWrite = 0;

    while (true)
    {
        uint16_t mappedSpeed = map(valueToWrite, 0, 500, 0, DAC_MAP_VALUE);
        Log.traceln("valueToWrite = %d", valueToWrite);
        dacWrite(DAC0, mappedSpeed);
        vTaskDelay(10 / portTICK_PERIOD_MS);
        valueToWrite++;
        if (valueToWrite >= 500)
        {
            valueToWrite = 0;
            vTaskSuspend(Task2);
            vTaskSuspend(Task1);
        }
    }
}

void Task2code(void *parameter)
{
    Log.traceln("Task2 is running on core %d", xPortGetCoreID());

    while (true)
    {
        int analogIn = analogRead(ADC_IN);
        uint16_t mappedAnalogIn = map(analogIn, 0, 4095, 0, 500);
        Log.traceln("mappedAnalogIn = %d", mappedAnalogIn);
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void setupFreeRtos()
{
    Serial.begin(9600);
    Log.begin(LOG_LEVEL_TRACE, &Serial, true);
    // pinMode(ADC_IN, INPUT_PULLDOWN);
    pinMode(DAC0, OUTPUT);
    analogReadResolution(12);

    // 0 mV ~ 2500 mV
    analogSetAttenuation(ADC_11db);

    xTaskCreatePinnedToCore(Task1code, "Task1", 10000, NULL, 1, &Task1, 0);
    delay(500);

    xTaskCreatePinnedToCore(Task2code, "Task2", 10000, NULL, 1, &Task2, 1);
    delay(500);
}

void loopFreeRtos()
{
    // uint16_t valueToWrite = 0;
    // while (true)
    // {
    //     uint16_t mappedSpeed = map(valueToWrite, 0, 500, 0, 255);
    //     dacWrite(DAC0, mappedSpeed);

    //     int analogIn = analogRead(ADC_IN);
    //     uint16_t mappedAnalogIn = map(analogIn, 0, 4095, 0, 500);

    //     Log.traceln("valueToWrite = %d, mappedAnalogIn = %d", valueToWrite, mappedAnalogIn);

    //     valueToWrite++;
    //     if (valueToWrite >= 500)
    //     {
    //         break;
    //     }
    // }
}