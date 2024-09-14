//
// A simple server implementation showing how to:
//  * serve static messages
//  * read GET and POST parameters
//  * handle missing pages / 404s
//

#include <Arduino.h>
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>
#include "WifiInfo.h"
AsyncWebServer server(80);

const char *ssid = WIFIINFO_SSID;
const char *password = WIFIINFO_PASSWORD;

const char *PARAM_MESSAGE = "message";

void print_connection_info();
void getRequestHandler(AsyncWebServerRequest *request);
void postRequestHandler(AsyncWebServerRequest *request);

void notFound(AsyncWebServerRequest *request)
{
    request->send(404, "text/plain", "Not found");
}

void setup()
{
    Serial.begin(115200);

    delay(1000);
    Serial.println("Starting Setup...");
    //WiFi.setHostname("ESP32-Server");
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(1000);
    Serial.print("Connecting to WiFi ..");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    if (WiFi.waitForConnectResult() != WL_CONNECTED)
    {
        Serial.printf("WiFi Failed!\n");
        return;
    }

    print_connection_info();

    // GET Handler
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { getRequestHandler(request); });

    // POST Handler
    server.on("/", HTTP_POST, [](AsyncWebServerRequest *request)
              { postRequestHandler(request); });

    server.onNotFound(notFound);

    server.begin();
}

void postRequestHandler(AsyncWebServerRequest *request)
{
    String message;
    if (request->hasParam(PARAM_MESSAGE, true))
    {
        message = request->getParam(PARAM_MESSAGE, true)->value();
    }
    else
    {
        message = "No message sent";
    }
    request->send(200, "text/plain", "Post message received: " + message);
}

void getRequestHandler(AsyncWebServerRequest *request)
{
    String message;
    if (request->hasParam(PARAM_MESSAGE))
    {
        message = request->getParam(PARAM_MESSAGE)->value();
    }
    else
    {
        message = "No Message.";
    }
    request->send(200, "text/plain", "Get message received: " + message);
}

void loop()
{
}

void print_connection_info()
{
    Serial.print("ESP Hostname:");
    Serial.println(WiFi.getHostname());
    Serial.print("Connected to WiFi ..");
    Serial.println(ssid);
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
}
