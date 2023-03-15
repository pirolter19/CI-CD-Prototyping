#ifndef _HTTP_SERVER_HANDLER_H_
#define _HTTP_SERVER_HANDLER_H_

#include "ESPAsyncWebServer.h"
#include "Adafruit_NeoPixel.h"

#define LED_PIN 18
#define NUM_LEDS 24

// initialize async web server for port 80
void InitWebServer();

// returns the current web server object
AsyncWebServer GetWebServer();

extern int feedbackstatus;

#endif /* _HTTP_SERVER_HANDLER_H_ */