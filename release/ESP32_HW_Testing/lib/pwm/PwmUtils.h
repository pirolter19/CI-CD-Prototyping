/*
 * File:         PwmUtils.h
 * Author:       Philipp Steinwender
 * Created:      05.12.2022
 */

#ifndef PWM_UTILS_H
#define PWM_UTILS_H

#include <Arduino.h>
#include <ArduinoLog.h>

class PwmUtils
{
public:
    PwmUtils();

    // calculate duty cycle from given input-pin
    int calculateDutyCyclePulseIn(uint8_t inputPin);

private:
};
#endif