/*
 * File:         pinout_esp.h
 * Author:       Philipp Steinwender
 * Created:      11.01.2023
 */

#ifndef PINOUT_ESP_H
#define PINOUT_ESP_H

// DC motor
#define DC_MOTOR_DIR_A_IN 7        // counter-clockwise (uno pin 3)
#define DC_MOTOR_DIR_B_IN 8        // clockwise (uno pin 4)
#define DC_MOTOR_ENABLE_IN 9       // off (uno pin 5)
#define DC_MOTOR_ANALOG_DIR_OUT 17 // joystick value for direction (uno pin A0)

// Servo
#define SERVO_PWM_PIN_IN 6 // pwm send from uno to servo (uno pin 6)

// pin with 220ohm resistor to ground,
// otherwise offset of 1.2V
#define SERVO_ANALOG_POSITION_OUT 18 // joystick value for positioning (uno pin A1)

// Ultrasonic sensor
#define USONIC_TRIGGER_PIN_IN 5 // triggers the ultrasonic sensor (uno pin 9)
#define USONIC_ECHO_PIN_OUT 4   // echo returned from ultrasonic sensor (uno pin 8)

// Leds (distance indicators)
#define LED_GREEN_PIN 3  // (uno pin 12)
#define LED_YELLOW_PIN 2 // (uno pin 11)
#define LED_RED_PIN 1    // (uno pin 10)

// Serial reader
#define SERIAL_RX 10 // (uno pin A4)
#define SERIAL_TX 11 // (uno pin A5)

#endif // PINOUT_ESP_H