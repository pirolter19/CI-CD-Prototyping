#ifndef PINOUT_UNO_H
#define PINOUT_UNO_H

// Software Serial
#define RX_PIN A5 // esp 11
#define TX_PIN A4 // esp 10

// LED Pins
#define LED_GREEN_PIN 12
#define LED_YELLOW_PIN 11
#define LED_RED_PIN 10

// Ultra Sonic Pins
#define USONIC_TRIGGER_PIN 9
#define USONIC_ECHO_PIN 8

// Servo Motor Pins
#define SERVO_PWM_PIN 6      // esp 6
#define SERVO_POSITION_IN A1 // esp 18

// DC Motor Pins
#define DC_MOTOR_DIR_A 3   //  esp 7
#define DC_MOTOR_DIR_B 4   // esp 8
#define DC_MOTOR_ENABLE 5  // esp 9
#define DC_MOTOR_DIR_IN A0 // esp 17

// Software Serial
#define RX A0
#define TX A1

#endif // PINOUT_UNO_H
