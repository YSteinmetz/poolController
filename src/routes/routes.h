#ifndef ROUTES_H
#define ROUTES_H

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

class routes{
    public:
        routes(ESP8266WebServer *server);
        void begin();
    private:
        ESP8266WebServer *_server;
        void handleRoot();

};

routes::routes(ESP8266WebServer *server){
    _server = server;
}

void routes::begin(){
    
    _server->on("/", std::bind(&routes::handleRoot, this));
  
}
#endif