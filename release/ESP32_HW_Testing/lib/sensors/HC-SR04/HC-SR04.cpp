/// @file

#include "HC-SR04.h"

extern uint16_t expectedDistance; // cm
extern uint8_t usonic_echo;
extern uint8_t usonic_trigger;
bool wait_isr;

void IRAM_ATTR handleInterrupt(void)
{
    delayMicroseconds(100);

    digitalWrite(usonic_echo, HIGH);

    uint64_t timestamp = micros();
    uint64_t timestamp_to_low = timestamp + expectedDistance * 58;

    while (micros() < timestamp_to_low)
    {
    }

    digitalWrite(usonic_echo, LOW);
    wait_isr = true;
}

/// @brief This functions sets the output and inputs needed for emulating the ultrasonic HC-SR04 sensor 
/// @param usonic_echo_pin_out Provide the pin number this signal is connected to the ESP
/// @param usonic_trigger_pin_in Provide the pin number this signal is connected to the ESP
void hc_sr04_setup(char usonic_echo_pin_out, char usonic_trigger_pin_in)
{
    pinMode(usonic_echo_pin_out, OUTPUT); // HC-SR04 Pins
    pinMode(usonic_trigger_pin_in, INPUT);

    usonic_echo = usonic_echo_pin_out;
    usonic_trigger = usonic_trigger_pin_in;
}

/// @brief This functions emulates the signal on the echo pin of the HC-SR04 for a certain distance
/// @param distance Provide the distance value you want to emulate
void hc_sr04_emulate(uint16_t distance)
{
    expectedDistance = distance;
    attachInterrupt(usonic_trigger, handleInterrupt, FALLING);
    while(!wait_isr);
    wait_isr = false;
    detachInterrupt(usonic_trigger);
}