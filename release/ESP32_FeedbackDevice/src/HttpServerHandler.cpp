#include "HttpServerHandler.h"

// prototypes
void SetupHttpWebServer();
void PostRequestDataAction(AsyncWebServerRequest *request, uint8_t *data, size_t len);
void PostRequestAction(AsyncWebServerRequest *request);
void PostRequestActionFBD(AsyncWebServerRequest *request);

// globals
AsyncWebServer server(80); // create webserver for http requests

int feedbackstatus = 2;

void InitWebServer()
{
    Serial.println("Init web server...");
    SetupHttpWebServer();
}

AsyncWebServer GetWebServer()
{
    return server;
}

void SetupHttpWebServer()
{
    // setup async webServer for post requests
    server.on(
        "/post",
        HTTP_POST,
        [](AsyncWebServerRequest *request)
        {
            PostRequestAction(request);
            PostRequestActionFBD(request);
        },
        NULL,
        [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total)
        {
            PostRequestDataAction(request, data, len);
        });

    server.begin();
}

// Contains functionalities for simple post request with data
void PostRequestDataAction(AsyncWebServerRequest *request, uint8_t *data, size_t len)
{
    Serial.println("PostRequestAction");

    for (size_t i = 0; i < len; i++)
    {
        Serial.write(data[i]);
    }

    Serial.println();
    request->send(200);
}

// Contains functionalities for simple post request
// Check status from jenkins and set success flag
// curl --location --request POST http://192.168.116.22/post?status=success
void PostRequestAction(AsyncWebServerRequest *request)
{
    Serial.println("PostRequestAction");

    bool success = false;

    // Check if parameter exists and print its argument
    if (request->hasArg("status"))
    {
        String arg = request->arg("status");
        if (arg == "success")
        {
            success = true;
        }

        request->send(200);
    }
    else
    {
        request->send(400);
        return;
    }

    if (success)
    {
        Serial.println("run was successful");
        // call method if run was successful
    }
    else
    {
        Serial.println("run failed");
        // call method if run failed
    }
}

void PostRequestActionFBD(AsyncWebServerRequest *request)
{
    Serial.println("PostRequestActionFBD");

    // Check if parameters exists and print its argument
    if (request->hasArg("status"))
    {
        feedbackstatus = request->arg("status").toInt();
        Serial.println(feedbackstatus);

        if (feedbackstatus == 0)

            Serial.print("FAILED\n");

        else if (feedbackstatus == 1)
            Serial.print("OK\n");

        else
            Serial.print("STATUS PENDING\n");
    }
    request->send(200);
}
