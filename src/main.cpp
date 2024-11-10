/*
 * WebSocketClient.ino
 *
 *  Created on: 24.05.2015
 *
 */
#define DEBUG_WEBSOCKETS(...) Serial.printf(__VA_ARGS__)
#define USE_SERIAL Serial
#define DEBUG_ESP_PORT USE_SERIAL

#include <Arduino.h>

#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiClientSecure.h>

#include <WebSocketsClient.h>

WebSocketsClient webSocket;

// WiFi credentials
const char *ssid = "BH24";
const char *password = "zuperman";

void webSocketEvent(WStype_t type, uint8_t *payload, size_t length)
{
  switch (type)
  {
  case WStype_DISCONNECTED:
    Serial.printf("[WSc] Disconnected!\n");
    break;
  case WStype_CONNECTED:
  {
    Serial.printf("[WSc] Connected to url: %s\n", payload);
    // send message to server when Connected
    webSocket.sendTXT("{\"status\": \"connected\"}");
    break;
  }
  case WStype_TEXT:
    Serial.printf("[WSc] get text: %s\n", payload);
    // send message to server
    // webSocket.sendTXT("message here");
    break;
  case WStype_BIN:
    Serial.printf("[WSc] get binary length: %u\n", length);
    break;
  case WStype_ERROR:
    Serial.printf("[WSc] Error: %s\n", payload);
    break;
  case WStype_PING:
    Serial.printf("[WSc] Got Ping\n");
    break;
  case WStype_PONG:
    Serial.printf("[WSc] Got Pong\n");
    break;
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println();

  // Connect to WiFi
  Serial.printf("Connecting to %s\n", ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.printf("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  // server address, port and URL
  webSocket.begin("192.168.100.101", 4000, "/socket/api/v1/websocket","phoenix");

  // event handler
  webSocket.onEvent(webSocketEvent);

  // try ever 5000 again if connection has failed
  webSocket.setReconnectInterval(5000);


  // Set basic auth if needed
  // webSocket.setAuthorization("user", "Password");
}

void loop()
{
  webSocket.loop();
}