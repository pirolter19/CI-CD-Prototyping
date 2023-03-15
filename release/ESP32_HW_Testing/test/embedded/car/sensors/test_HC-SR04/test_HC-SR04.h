/*
 * File:         test_HC-SR04.h
 * Author:       Andreas Fischer
 * Created:      04.01.2023
 */

#ifndef TEST_HCSR04
#define TEST_HCSR04

#include "Arduino.h"
#include "SerialReader.cpp"
#include "pinout_esp.h"

#define delay_time 3000
#define TIMEOUT_MS 10000

// Set up a new SoftwareSerial object
// #define rxPin 10
// #define txPin 11
// #define baudRate 9600

SerialReader serialReader(10, 11, 9600);

uint16_t expectedDistance = 0; // cm

void IRAM_ATTR handleInterrupt(void); // ISR function

void test_distance_20(void);
void test_distance_40(void);
void test_distance_75(void);
void test_distance_120(void);

int runUnityTests(void);

#endif // TEST_HCSR04