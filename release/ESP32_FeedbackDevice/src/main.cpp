
//////////////////////////////////
// ECE-Project group D          //
// author: Niklas Steiner       //
// date:   21.01.2023           //
//////////////////////////////////

#include <Arduino.h>
#include <WiFi.h>
#include <HttpServerHandler.h>

#define SIREN_PIN 7
#define LED_PIN 18
#define NUM_LEDS 24
#define MAX_BRIGHTNESS 100
#define DEFAULT_BRIGHTNESS 50

// prototypes
void OnWiFiEvent(WiFiEvent_t event);
void SetupWifiConnection();

// Neopixel ring
void pixelWipe(uint32_t c, uint16_t interval);
void pixelError(uint32_t c, uint16_t interval);
void pixelPulse(uint32_t color, uint16_t pulseDelay);

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB);
uint16_t pixelErrorQ;
uint16_t currentPixel;             // the pixel we are operating on
unsigned long pixelsInterval = 60; // the time we need to wait
unsigned long previousMillis;
extern int feedbackstatus; // received status from HTTP server
uint8_t brightness = 0;
uint8_t brightnessDirection = 1;
uint8_t state;
bool isOn = true;

// globals
const char *ssid = "Not a Hotspot";    // ssid of router Spying_On_Fh
const char *password = "861ea7fc719f"; // psw of router ECE20groupD

// Set your Static IP address
IPAddress local_IP(192, 168, 34, 200);
// Set your Gateway IP address
IPAddress gateway(192, 168, 34, 136);

IPAddress subnet(255, 255, 255, 0);

void setup()
{
  // init serial connection
  Serial.begin(115200);
  Serial.println("Initialize System");

  // Configures static IP address
  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("STA Failed to configure");
  }

  // setup and connect wifi
  SetupWifiConnection();

  // setup webserver to handle http requests
  InitWebServer();

  // Neopixel ring setup
  strip.begin();
  strip.setBrightness(DEFAULT_BRIGHTNESS);
  strip.show(); // Initialize all pixels to 'off'

  // setup siren pin
  pinMode(SIREN_PIN, OUTPUT);
}

void loop()
{
  if (feedbackstatus == 0)
  {
    digitalWrite(SIREN_PIN, HIGH);
    pixelError(strip.Color(255, 0, 0), pixelsInterval);
  }
  else if (feedbackstatus == 1)
  {
    digitalWrite(SIREN_PIN, LOW);
    pixelWipe(strip.Color(0, 255, 0), pixelsInterval); // green
  }
  else
  {
    digitalWrite(SIREN_PIN, LOW);
    pixelPulse(strip.Color(20, 20, 150), 30);
  }

  delay(50);
}

// set to station mode (esp will be connected to another wifi)
// connect to wifi with give ssid and psw
// wait until connected
// print ip of esp
void SetupWifiConnection()
{
  WiFi.mode(WIFI_MODE_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }

  Serial.println(WiFi.localIP());
}

void pixelError(uint32_t c, uint16_t interval)
{
  strip.setBrightness(DEFAULT_BRIGHTNESS);

  unsigned long currentMillis = millis();
  if ((currentMillis - previousMillis) >= interval)
  {
    previousMillis = currentMillis;
    for (int i = 0; i < NUM_LEDS; i++)
    {
      if ((i + pixelErrorQ) % 3 == 0)
      {
        strip.setPixelColor(i, c);
      }
      else
      {
        strip.setPixelColor(i, 0);
      }
    }
    strip.show();
    pixelErrorQ++;
    if (pixelErrorQ >= 3)
    {
      pixelErrorQ = 0;
    }
  }
}

void pixelWipe(uint32_t c, uint16_t interval)
{
  strip.setBrightness(DEFAULT_BRIGHTNESS);

  unsigned long currentMillis = millis();
  if ((currentMillis - previousMillis) >= interval)
  {
    previousMillis = currentMillis;
    if (isOn)
    {
      strip.setPixelColor(currentPixel, c);
    }
    else
    {
      strip.setPixelColor(currentPixel, 0);
    }
    strip.show();
    currentPixel++;
    if (currentPixel == NUM_LEDS)
    {
      currentPixel = 0;
      isOn = !isOn;
    }
  }
}

void pixelPulse(uint32_t color, uint16_t pulseDelay)
{
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= pulseDelay)
  {
    previousMillis = currentMillis;

    // Update the brightness
    brightness += brightnessDirection;
    if (brightness >= MAX_BRIGHTNESS || brightness <= 0)
    {
      brightnessDirection = -brightnessDirection;
    }

    // Set the color and brightness for all LEDs
    strip.setBrightness(brightness);
    for (int i = 0; i < NUM_LEDS; i++)
    {
      strip.setPixelColor(i, color);
    }
    strip.show();
  }
}
