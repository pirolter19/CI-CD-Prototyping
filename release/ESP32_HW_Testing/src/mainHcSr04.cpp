// /* --- ESP32 --- */

// #include <Arduino.h>

// #define TRIGGER_PIN 5
// #define ECHO_PIN 4

// uint16_t expectedDistance = 100; // cm
// #define constant 58      // constant multiplier

// unsigned long timestamp = 0;
// unsigned long timestamp_to_low = 0;

// void IRAM_ATTR handleInterrupt();

// void setupHcSr04()
// {
//   Serial.begin(9600);

//   pinMode(ECHO_PIN, OUTPUT);
//   pinMode(TRIGGER_PIN, INPUT);

//   // Attach an interrupt to TRIGGER_PIN pin
//   attachInterrupt(digitalPinToInterrupt(TRIGGER_PIN), handleInterrupt, CHANGE);

//   Serial.println("*** started ***");
// }

// void loopHcSr04()
// {
//   if (micros() > timestamp_to_low)
//   {
//     digitalWrite(ECHO_PIN, LOW);
//   }
// }

// void IRAM_ATTR handleInterrupt()
// {
//   delayMicroseconds(100);

//   digitalWrite(ECHO_PIN, HIGH);

//   timestamp = micros();
//   timestamp_to_low = timestamp + expectedDistance * constant;
// }