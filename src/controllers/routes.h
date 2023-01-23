#ifndef ROUTES_H
#define ROUTES_H

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "./controllers/webServerController.h"

class routes{
    public:
        routes(ESP8266WebServer *server);
        void begin();
    private:
        ESP8266WebServer *_server;
        void handleRoot();
        void handleNotFound();
};


routes::routes(ESP8266WebServer *server) {
    _server = server;
}

void routes::begin(){
    
    _server->on("/", std::bind(&routes::handleRoot, this));
    _server->onNotFound(std::bind(&routes::handleNotFound, this));
    _server->on("/main.js", [this](){
        _server->sendHeader("Content-Type", "application/javascript");
        File jsFile = SPIFFS.open("/main.js", "r");
        _server->streamFile(jsFile, "application/javascript");
        jsFile.close();
    });
  
}

void routes::handleRoot() {
    String html = "<h1>Hello from ESP8266!</h1>";
    _server->send(200, "text/html", html);
}


void routes::handleNotFound() {
    String html = "<h1>404 - Not Found</h1>";
    _server->send(404, "text/html", html);
}

#endif
