#include "./controllers/webServerController.h"

int port = 80;
webServerController mserver("FIBRA-FB74","0Z37006587", port);

void setup(void){
  mserver.begin();
}

void loop(void){
  mserver.handleClient();
}
