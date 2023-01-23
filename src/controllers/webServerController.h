#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "routes.h"

class webServerController
{
private:

  ESP8266WebServer _server;
  routes _router;
  String _ssid;
  String _password;
public:
  webServerController(String ssid, String password, int port);
  
  void begin();
  void handleClient();
};

webServerController::webServerController(String ssid, String password, int port) :
_server(port), _router(&_server)
{
  _ssid = ssid;
  _password = password;
}

void webServerController::begin(){

  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.begin(_ssid.c_str(), _password.c_str());

  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(_ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }
  
  //Server routes and start region.

  _router.begin();

  Serial.println("HTTP server started");
  MDNS.addService("http", "tcp", 80);

}

void webServerController::handleClient(){
  _server.handleClient();
  MDNS.update();
}

#endif // WEB_SERVER_H
