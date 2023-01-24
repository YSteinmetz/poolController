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

  ESP8266WebServer server;
  routes router;
  String _ssid;
  String _password;
  void handleRoot();
  void handleNotFound();
public:
  webServerController(String ssid, String password, int port);
  
  void begin();
  void handleClient();
};

webServerController::webServerController(String ssid, String password, int port) :
server(port), router(&server){
  _ssid = ssid;
  _password = password;
}

void webServerController::begin(){

  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(_ssid, _password);
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

  router.begin();
  //server.on("/", std::bind(&webServerController::handleRoot, this));
  //server.onNotFound(std::bind(&webServerController::handleNotFound, this));

  server.begin();
  Serial.println("HTTP server started");

}

void webServerController::handleClient(){
  server.handleClient();
}


#endif // WEB_SERVER_H
