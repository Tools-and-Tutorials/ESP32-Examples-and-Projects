#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#include "WiFiInfo.h"

void connectToWifi();
String getTimeFromServer();
String getWeatherFromServer(String);

float latitude = 51.5085;
float longitude = -0.1257;

void setup()
{
  Serial.begin(115200);
  Serial.println("Staring device....");
  connectToWifi();
}

void loop()
{
  String time=getTimeFromServer();
  String weather=getWeatherFromServer(time.substring(0,14)+"00");

  delay(1000 * 60 * 60);
}

void connectToWifi()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

String getTimeFromServer()
{
  HTTPClient http;
  Serial.print("[HTTP] begin...\n");
  http.begin("http://worldclockapi.com/api/json/gmt/now"); // Example: {"$id":"1","currentDateTime":"2024-09-04T22:33+01:00","utcOffset":"01:00:00","isDayLightSavingsTime":true,"dayOfTheWeek":"Wednesday","timeZoneName":"GMT Standard Time","currentFileTime":133699627820269036,"ordinalDate":"2024-248","serviceResponse":null}
  int httpCode = http.GET();
  if (httpCode > 0)
  {
    String payload = http.getString();
    JsonDocument doc;
    deserializeJson(doc, payload);
    String datetime = doc["currentDateTime"];
    Serial.print("Time from server: ");
    Serial.println(datetime);
    String resp = doc["serviceResponse"];
    if (resp != NULL)
      Serial.println(resp);
    return datetime;
  }
  else
  {
    Serial.println("Error getting time from server");
    return "";
  }
  http.end();
}

String getWeatherFromServer(String time)
{
  HTTPClient http;
  Serial.print("[HTTP] fetch for time \n");
  Serial.println(time);
  String url = "https://api.open-meteo.com/v1/forecast?latitude=" + String(latitude) + "&longitude=" + String(longitude) + "&hourly=temperature_2m&forecast_days=1";
  http.begin(url); //{"latitude":9.875,"longitude":78.125,"generationtime_ms":0.033020973205566406,"utc_offset_seconds":0,"timezone":"GMT","timezone_abbreviation":"GMT","elevation":139.0,"hourly_units":{"time":"iso8601","temperature_2m":"°C"},"hourly":{"time":["2024-09-04T00:00","2024-09-04T01:00","2024-09-04T02:00","2024-09-04T03:00","2024-09-04T04:00","2024-09-04T05:00","2024-09-04T06:00","2024-09-04T07:00","2024-09-04T08:00","2024-09-04T09:00","2024-09-04T10:00","2024-09-04T11:00","2024-09-04T12:00","2024-09-04T13:00","2024-09-04T14:00","2024-09-04T15:00","2024-09-04T16:00","2024-09-04T17:00","2024-09-04T18:00","2024-09-04T19:00","2024-09-04T20:00","2024-09-04T21:00","2024-09-04T22:00","2024-09-04T23:00"],"temperature_2m":[26.1,26.0,27.3,29.5,31.6,33.7,35.3,36.8,37.3,37.7,36.5,35.9,34.2,31.9,30.1,29.0,28.5,28.1,28.2,28.2,27.9,27.2,26.9,26.7]}}
  int httpCode = http.GET();
  if (httpCode > 0)
  {
    String payload = http.getString();
    JsonDocument doc;
    deserializeJson(doc, payload);
    JsonArray datetime = doc["hourly"]["time"];
    Serial.println("Date time list:");
    Serial.println(datetime);
    for(int i=0;i<datetime.size();i++)
    {
      Serial.println(datetime[i].as<String>());
      if(datetime[i].as<String>()==time)
      {
        Serial.print("Index of time: ");
        Serial.println(i);
        Serial.print("Temperature: ");
        Serial.println(doc["hourly"]["temperature_2m"][i].as<String>());
        Serial.print("Temperature unit: ");
        Serial.println(doc["hourly_units"]["temperature_2m"].as<String>());
        return doc["hourly"]["temperature_2m"][i].as<String>()+doc["hourly_units"]["temperature_2m"].as<String>();
      }
    }
    return "N/A";
  }
  else
  {
    Serial.println("Error getting time from server");
    return "SE";
  }
  http.end();
}