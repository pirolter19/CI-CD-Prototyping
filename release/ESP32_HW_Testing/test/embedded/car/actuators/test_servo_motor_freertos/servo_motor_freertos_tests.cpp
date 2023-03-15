/*
 * File:         servo_motor_freertos_tests.cpp
 * Author:       Philipp Steinwender
 * Created:      29.01.2023
 */

#include <Arduino.h>
#include <ArduinoLog.h>
#include <unity.h>
#include "SerialReader.h"
#include "timeout_handler.h"
#include "pinout_esp.h"
#include "serial_tags.h"
#include <FreeRTOS.h>

// defines
#define TimeOut 20000     // timeout for tests
#define DAC_MAP_VALUE 280 // 255 ~ 3.3V

// freertos tasks
static TaskHandle_t WriteTask;
static TaskHandle_t ReadTask;

// serial reader for checking printed values on uno
SerialReader serialReader(10, 11, 9600);

int actualServoValue = 0;
int servoPosition = 0;

void setUp(void)
{
    // setup serial for test logging
    Serial.begin(9600);

    // use ArduinoLog with LogLevel
    Log.begin(LOG_LEVEL_TRACE, &Serial, false);

    // set servo position pin as output (joystick)
    pinMode(SERVO_ANALOG_POSITION_OUT, OUTPUT);
}

void setup()
{
    // NOTE!!! Wait for > 2 secs
    delay(3000);

    // call unity setup function
    setUp();

    delay(2000);
    runUnityTests();
}

void tearDown(void)
{
    vTaskDelete(ReadTask);
    vTaskDelete(WriteTask);
}

int runUnityTests(void)
{
    UNITY_BEGIN();

    // set and start timeout
    TimeoutHandler timeoutHandler(TimeOut);
    timeoutHandler.start();

    // run tests until finished or timeout occurred
    while (!timeoutHandler.hasTimeout())
    {
        RUN_TEST(set_servo_position_10_170_freertos);
        break;
    }

    return UNITY_END();
}

// calculate mean value of read values for given sample size
int getMeanValueOfReadValues(uint8_t samples)
{
    int sum = 0;
    for (int i = 0; i < samples;)
    {
        int readValue = atoi(serialReader.read(SERVO_SERIAL_TAG));
        Log.verboseln("readValue = %d", readValue);

        // only use values bigger zero, because we don't test zero level
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
    // map the given servo position to the DAC range
    uint16_t mappedValueToWrite = map(servoPosition, 0, 180, 0, DAC_MAP_VALUE);

    Log.verboseln("mappedValueToWrite = %d", mappedValueToWrite);

    // write the mapped value with the DAC
    dacWrite(SERVO_ANALOG_POSITION_OUT, mappedValueToWrite);
}

// function which is pinned to freeRTOS task (has to have this signature)
void writeServoPosition(void *parameter)
{
    vTaskDelay(100 / portTICK_PERIOD_MS);

    servoPosition = 10;
    while (servoPosition < 170)
    {
        writeDacValue(servoPosition);
        
        servoPosition += 10;
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }

    // delete tasks after we finished writing values
    vTaskDelete(ReadTask);
    vTaskDelete(WriteTask);
}

// function which is pinned to freeRTOS task (has to have this signature)
void readServoPosition(void *parameter)
{
    // run until the task is suspended or deleted
    while (true)
    {
        // calc mean of printed values
        actualServoValue = getMeanValueOfReadValues(5);
        Log.traceln("delta=5\texpected=%d\tactual=%d", servoPosition, actualServoValue);

        // check if the actual value is between a range of +- 5
        TEST_ASSERT_UINT32_WITHIN(5, servoPosition, actualServoValue);
    }
}

void set_servo_position_10_170_freertos()
{
    Log.traceln("running set_servo_position_10_170_freertos");

    // pin read task to core 1
    xTaskCreatePinnedToCore(readServoPosition, "ReadTask", 65536, NULL, 1, &ReadTask, 1);
    delay(20);

    // pin write task to core 0
    xTaskCreatePinnedToCore(writeServoPosition, "WriteTask", 10000, NULL, 1, &WriteTask, 0);
    delay(20);

    // wait until tasks are finished -> deleted
    while (eTaskGetState(WriteTask) != eDeleted && eTaskGetState(ReadTask) != eDeleted)
    {
    }
}

void loop()
{
}
