#include "./controllers/webServerController.h"
#include "credentials.h"

int port = 80;
webServerController mserver(WIFI_SSID,WIFI_PASSWORD, port);

void setup(void){
  mserver.begin();
}

void loop(void){
  mserver.handleClient();
}
