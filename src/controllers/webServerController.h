#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <./controllers/poolMotorController.h>
#include <./routes/routes.h>

class WebServer {
  public:
    WebServer(const char* host, const char* ssid, const char* password, int port, PoolMotorController &motorController);
    void handleClient();
    bool wifiStatus();
    void begin();
  private:
    ESP8266WebServer _server;
    PoolMotorController &_motorController;
    routes _routes;
    bool _apMode = false;
    const char* _ssid;
    const char* _password;
    const char* _host;
    
};

WebServer::WebServer(const char* host, const char* ssid, const char* password, int port, PoolMotorController &motorController)
: _server(port), _motorController(motorController), _routes(&_server,motorController) {

  _ssid = ssid;
  _password = password;
  _host = host;
  
}

void WebServer::begin(){
  Serial.begin(9600, SERIAL_8N1);

  WiFi.begin(_ssid, _password);
  int timeout = 0;

  Serial.println("Connecting to WiFi...");
  
  while (WiFi.status() != WL_CONNECTED && timeout < 30) {
    delay(1000);
    timeout++;
  }

  if(WiFi.status() != WL_CONNECTED){
    Serial.println("WiFi connection error");
    return;
  }

  Serial.println("Connected to WiFi");

  _routes.begin();

    if (MDNS.begin(_host)) {
    Serial.println("MDNS responder started");
  }

  
  Serial.println("HTTP server started");

  MDNS.addService("http", "tcp", 80);

}

bool WebServer::wifiStatus(){

  if(WiFi.status() != WL_CONNECTED){
    return false;
  }
  return true;
}

void WebServer::handleClient() {
    _server.handleClient();
}


#endif // WEB_SERVER_H