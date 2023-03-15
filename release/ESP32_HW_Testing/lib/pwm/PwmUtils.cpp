/*
 * File:         PwmUtils.cpp
 * Author:       Philipp Steinwender
 * Created:      05.12.2022
 */

#include <PwmUtils.h>

PwmUtils::PwmUtils()
{
}

int PwmUtils::calculateDutyCyclePulseIn(uint8_t inputPin)
{
    // get high time in ms
    int onTime = pulseIn(inputPin, HIGH);

    // get low time in ms
    int offTime = pulseIn(inputPin, LOW);

    // sum up for period time
    float period = onTime + offTime;

    // calculate frequency from ms period
    float freq = 1000000.0 / period;

    // calculate duty cycle in %
    int dutyCycle = (onTime / period) * 100;

    // check if value is valid
    if (dutyCycle < 0 || dutyCycle > 100)
    {
        Log.errorln("invalid dutycycle = %d", dutyCycle);
        return -1;
    }

    Log.verboseln("dutycycle = %d", dutyCycle);
    return dutyCycle;
}
