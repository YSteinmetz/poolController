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
server(port){
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

  server.on("/", std::bind(&webServerController::handleRoot, this));
  server.onNotFound(std::bind(&webServerController::handleNotFound, this));

  server.begin();
  Serial.println("HTTP server started");

}

void webServerController::handleClient(){
  server.handleClient();
}



void webServerController::handleRoot() {
  server.send(200, "text/plain", "hello from esp8266!");

}

void webServerController::handleNotFound(){

  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);

}
#endif // WEB_SERVER_H
