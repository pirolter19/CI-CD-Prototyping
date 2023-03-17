/// @file
#include "PwmReader.h"

// use volatile for variables changed in ISR

/// @brief an array to store pulsewidth measurements
volatile int pwmMeasurements[numberOfPins];

/// @brief the time of the current pin change interrupt
volatile uint64_t changeInterruptTime;

/// @brief an array to store the start time of each getPulseWidthLow pulse
volatile uint64_t pwmPulseStartTimes[numberOfPins];

/// @brief flag whenever new data is available on each pin
volatile bool pwmNewDataFlag[numberOfPins];

/// @brief period, mirco sec, between two pulses on each pin
volatile uint64_t pwmPeriods[numberOfPins];

/// @brief an array used to determine whether a pin has gone low-high or high-low
volatile bool prevPinState[numberOfPins];

/// @brief determines if rising edge of channel the channel was detected -> start
volatile bool pwmChannelReadyToRead[numberOfPins];

// globals
bool isInitialized = false;
uint64_t pwmPulseStartTime;
float pwmPulseWidthHigh;
float pwmPeriodTime;

void initPwmReader()
{
    uint8_t pinIndex = 0;
    for (pinIndex = 0; pinIndex < numberOfPins; pinIndex++)
    {
        // set pin as input
        pinMode(pwmPins[pinIndex], INPUT_PULLUP);

        // set default pin state
        prevPinState[pinIndex] = 0;

        // add ISR to pin
        setupPwmRisingInterrupt(pwmPins[pinIndex], pinIndex);
    }

    isInitialized = true;
    Log.traceln("pwm reader initialized.");
}

void setupPwmRisingInterrupt(uint8_t pin, uint8_t pinIndex)
{
    auto isr = pwmRisingISRs[pinIndex];
    attachInterrupt(pin, *isr, RISING);
}

void setupPwmChangingInterrupt(uint8_t pin, uint8_t pinIndex)
{
    auto isr = pwmChangeISRs[pinIndex];
    attachInterrupt(pin, *isr, CHANGE);
}

void pwmChangeAnalyze(uint8_t channel)
{
    // time in ms when pin change occured
    changeInterruptTime = micros();

    if (prevPinState[channel] == 0)
    {                                                                            // and the pin state has changed from LOW to HIGH (start of pulse)
        prevPinState[channel] = 1;                                               // record pin state
        pwmPeriods[channel] = changeInterruptTime - pwmPulseStartTimes[channel]; // calculate the time period, micro sec, between the current and previous pulse
        pwmPulseStartTimes[channel] = changeInterruptTime;                       // record the start time of the current pulse
    }
    else if (prevPinState[channel] == 1)
    {                                                                                 // or the pin state has changed from HIGH to LOW (end of pulse)
        prevPinState[channel] = 0;                                                    // record pin state
        pwmMeasurements[channel] = changeInterruptTime - pwmPulseStartTimes[channel]; // calculate the duration of the current pulse
        pwmNewDataFlag[channel] = HIGH;                                               // flag that new data is available
    }
}

void IRAM_ATTR pwmChangeChannel1ISR()
{
    pwmChangeAnalyze(0);
}

void IRAM_ATTR pwmChangeChannel2ISR()
{
    pwmChangeAnalyze(1);
}

void IRAM_ATTR pwmChangeChannel3ISR()
{
    pwmChangeAnalyze(2);
}

void IRAM_ATTR pwmChangeChannel4ISR()
{
    pwmChangeAnalyze(3);
}

void IRAM_ATTR pwmRisingChannel1ISR()
{
    uint8_t pin = 0;
    pwmChangeAnalyze(pin);
    pwmChannelReadyToRead[pin] = true;
    detachInterrupt(pwmPins[pin]);
    setupPwmChangingInterrupt(pwmPins[pin], pin);
}

void IRAM_ATTR pwmRisingChannel2ISR()
{
    uint8_t pin = 1;
    pwmChangeAnalyze(pin);
    pwmChannelReadyToRead[pin] = true;
    detachInterrupt(pwmPins[pin]);
    setupPwmChangingInterrupt(pwmPins[pin], pin);
}

void IRAM_ATTR pwmRisingChannel3ISR()
{
    uint8_t pin = 2;
    pwmChangeAnalyze(pin);
    pwmChannelReadyToRead[pin] = true;
    detachInterrupt(pwmPins[pin]);
    setupPwmChangingInterrupt(pwmPins[pin], pin);
}

void IRAM_ATTR pwmRisingChannel4ISR()
{
    uint8_t pin = 3;
    pwmChangeAnalyze(pin);
    pwmChannelReadyToRead[pin] = true;
    detachInterrupt(pwmPins[pin]);
    setupPwmChangingInterrupt(pwmPins[pin], pin);
}

bool readPwmData(uint8_t channel)
{
    // array index start 0 -> after all that time!!!!!!
    int channelIndex = channel - 1;

    if (!isInitialized)
    {
        Log.errorln("reader not initialized, call initPwmReader() first");
        return false;
    }

    if (!pwmChannelReadyToRead[channelIndex])
    {
        Log.errorln("first rising edge wasn't detected yet, no pwm reading possible for channel %d", channel);
        return false;
    }

    if (channel < 1 || channel > numberOfPins)
    {
        Log.errorln("given channel %d is outside the range 1 - %d", channel, numberOfPins);
        return false;
    }

    bool newDataAvailable = pwmNewDataFlag[channelIndex];
    if (newDataAvailable == HIGH)
    {
        pwmNewDataFlag[channelIndex] = LOW;
        noInterrupts();

        pwmPulseStartTime = pwmPulseStartTimes[channelIndex];
        Log.verboseln("pwmPulseStartTime = %l", pwmPulseStartTime);

        pwmPulseWidthHigh = pwmMeasurements[channelIndex];
        Log.verboseln("pwmPulseWidthHigh = %F", pwmPulseWidthHigh);

        pwmPeriodTime = pwmPeriods[channelIndex];
        Log.verboseln("pwmPeriodTime = %F", pwmPeriodTime);

        interrupts();
    }

    return newDataAvailable;
}

uint64_t getPwmPulseStartTime()
{
    return pwmPulseStartTime;
}

float getPwmPeriodTime()
{
    return pwmPeriodTime / 1000;
}

float getPulseWidthHigh()
{
    return pwmPulseWidthHigh / 1000;
}

float getPulseWidthLow()
{
    return getPwmPeriodTime() - getPulseWidthHigh();
}

float getPwmFrequency()
{
    float freq;
    return freq = 1000000 / pwmPeriodTime;
}

float getPwmDutyCycle()
{
    float duty;
    duty = pwmPulseWidthHigh / pwmPeriodTime * 100;
    return duty;
}
