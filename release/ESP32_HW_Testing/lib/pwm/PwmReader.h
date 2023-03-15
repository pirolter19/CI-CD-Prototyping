/*
 * File:         PwmReader.h
 * Author:       Philipp Steinwender
 * Created:      09.12.2022
 * @file
 */

#ifndef PWM_READER_H
#define PWM_READER_H

#include <Arduino.h>
#include <ArduinoLog.h>
#include "esp32-hal-gpio.h"

using namespace std;
using void_f = void (*)();

/// @brief  pins where pwm will be read
const int pwmPins[] = {18, 19, 20, 21};

/// @brief calculate the number of input pins
const int numberOfPins = sizeof(pwmPins) / sizeof(int);

// prototypes

/// @brief initialize pwm reader for all pins declared in pwmPins[] (main.cpp)
/// iterate over all pins, set them as input, and attach ISR
void initPwmReader();

/// @brief attach edge change ISR to given pin
void setupPwmChangeInterrupt(uint8_t pin, uint8_t pinIndex);

/// @brief attach edge rising ISR to given pin
void setupPwmRisingInterrupt(uint8_t pin, uint8_t pinIndex);

/// @brief analyze pwm data after interrupt (channels mapped to pwmPins[])
void pwmChangeAnalyze(uint8_t channel);

/// @brief ISR which will be called every edge change of channel 1 = pwmPins[0] (IRAM_ATTR -> use for ISR, if operations involve flash read/write.)
void IRAM_ATTR pwmChangeChannel1ISR();

/// @brief ISR which will be called every edge change of channel 2 = pwmPins[1] (IRAM_ATTR -> use for ISR, if operations involve flash read/write.)
void IRAM_ATTR pwmChangeChannel2ISR();

/// @brief ISR which will be called every edge change of channel 3 = pwmPins[2] (IRAM_ATTR -> use for ISR, if operations involve flash read/write.)
void IRAM_ATTR pwmChangeChannel3ISR();

/// @brief ISR which will be called every edge change of channel 4 = pwmPins[3] (IRAM_ATTR -> use for ISR, if operations involve flash read/write.)
void IRAM_ATTR pwmChangeChannel4ISR();

/// @brief ISR which will be called at the rising edge of channel 1 to detect the start of the transaction
void IRAM_ATTR pwmRisingChannel1ISR();

/// @brief ISR which will be called at the rising edge of channel 2 to detect the start of the transaction
void IRAM_ATTR pwmRisingChannel2ISR();

/// @brief ISR which will be called at the rising edge of channel 3 to detect the start of the transaction
void IRAM_ATTR pwmRisingChannel3ISR();

/// @brief ISR which will be called at the rising edge of channel 4 to detect the start of the transaction
void IRAM_ATTR pwmRisingChannel4ISR();

/// @brief read data of the channel and save values
/// @param channel to read
/// @return true if new data is available, false if no data available (error will be logged)
bool readPwmData(uint8_t channel);

/// @brief returns the time at the start of pulse
/// @return the time at the start of pulse
uint64_t getPwmPulseStartTime();

/// @brief returns the time between pulses
/// @return the time between pulses
float getPwmPeriodTime();

/// @brief returns the pulse width
/// @return the pulse width
float getPulseWidthLow();

/// @brief returns the pwm frequency in Hz
/// @return the pwm frequency in Hz
float getPwmFrequency();

/// @brief returns the pmw duty cycle
/// @return the pmw duty cycle
float getPwmDutyCycle();

/// @brief changing edge ISRs for different channels, mapped to pins.
void_f pwmChangeISRs[] =
    {
        pwmChangeChannel1ISR,
        pwmChangeChannel2ISR,
        pwmChangeChannel3ISR,
        pwmChangeChannel4ISR,
};

/// @brief rising edge ISRs for different channels, mapped to pins.
void_f pwmRisingISRs[] =
    {
        pwmRisingChannel1ISR,
        pwmRisingChannel2ISR,
        pwmRisingChannel3ISR,
        pwmRisingChannel4ISR,
};

#endif