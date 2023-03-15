/// @file

#ifndef _HC_SR04_H_
#define _HC_SR04_H_

#include "Arduino.h"

uint16_t expectedDistance; // cm
uint8_t usonic_echo;
uint8_t usonic_trigger;

void hc_sr04_setup(char usonic_echo_pin_out, char usonic_trigger_pin_in);
void hc_sr04_emulate(uint16_t distance);

#endif