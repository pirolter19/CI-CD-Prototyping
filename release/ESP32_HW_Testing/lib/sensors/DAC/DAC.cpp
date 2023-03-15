/// @file

#include "DAC.h"
extern uint8_t analog_value_pin;

/// @brief This functions sets the output where the dac value should be applied to
/// @param analog_value_pin_out Provide the pin number where the analog value should be written to. Make sure this pin has an ADC available.
void dac_setup(uint8_t analog_value_pin_out)
{
    pinMode(analog_value_pin_out, OUTPUT);
    analog_value_pin = analog_value_pin_out;
}

/// @brief This function applies the wished voltage to the output pin.
/// @param voltage Wished output voltage
/// @param reference_voltage Reference voltage of the ADCs
void dac_write(float voltage, float reference_voltage)
{
    uint8_t dac_value = (uint8_t)((voltage/reference_voltage) * 255.0);
    dacWrite(analog_value_pin, dac_value);
}
