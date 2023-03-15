/// @file

#ifndef _DAC_H_
#define _DAC_H_

#include "Arduino.h"

extern uint8_t analog_value_pin;

void dac_setup(uint8_t analog_value_pin_out);
void dac_write(float voltage, float reference_voltage);

#endif