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
    routes serverRoutes;
    bool _apMode = false;
    const char* _ssid;
    const char* _password;
    const char* _host;
    void handleRoot();
    void handleManualTurnOn();
    void handleManualTurnOff();
    void handleStartTimer();
    void handleSchedulePeriod();
    void handleNotFound();
    void createAP();
};

WebServer::WebServer(const char* host, const char* ssid, const char* password, int port, PoolMotorController &motorController)
: _server(port), _motorController(motorController), serverRoutes(&_server) {

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

    if (MDNS.begin(_host)) {
    Serial.println("MDNS responder started");
  }

  serverRoutes.begin();

  Serial.println("HTTP server started");

  MDNS.addService("http", "tcp", 80);

}


bool WebServer::wifiStatus(){

  if(WiFi.status() != WL_CONNECTED){
    return false;
  }
  return true;
}

void WebServer::handleManualTurnOn() {

  _motorController.manualTurnOn();
   _server.sendHeader("Location", "/");
   _server.send(302);
}

void WebServer::handleManualTurnOff() {

  _motorController.manualTurnOff();

   _server.sendHeader("Location", "/");
   _server.send(302);
}

void WebServer::handleStartTimer(){

  if(_server.argName(0) != "TON" || _server.argName(1) != "TOFF"){
    _server.send(400,"text/html", "bad request");
    return;
  }

  if(_server.arg(0).isEmpty() || _server.arg(1).isEmpty()){
    _server.send(400,"text/html", "bad request");
    return;
  }

  if(!isDigit(_server.arg(0).charAt(_server.arg(0).length() - 1)) || !isDigit(_server.arg(1).charAt(_server.arg(1).length() - 1))){
    _server.send(400,"text/html", "bad request");
    return;
  }

  _motorController.setTimer(_server.arg(0).toInt(),_server.arg(1).toInt());

  _server.sendHeader("Location", "/");
   _server.send(302); 
}

void WebServer :: handleSchedulePeriod(){

    String day = _server.arg("day");
    String start = _server.arg("start");
    String end = _server.arg("end");

    //validate input data
    if(day.isEmpty() || start.isEmpty() || end.isEmpty()){
        _server.send(400, "text/plain", "Bad Request: Missing parameter(s)");
        return;
    }

  //Check if the day is in the correct format
    if(day != "Monday" && day != "Tuesday" && day != "Wednesday" && day != "Thursday" && day != "Friday" && day != "Saturday" && day != "Sunday") {
        _server.send(400, "text/plain", "Bad Request: Bad parameter(s)");
        return;
    }

    //Check if the start and end times are in the correct format (HH:MM:SS)
    if(start.length() != 8 || end.length() != 8) {
        _server.send(400, "text/plain", "Bad Request: Bad parameter(s)");
        return;
    }
    if(start[2] != ':' || start[5] != ':' || end[2] != ':' || end[5] != ':') {
        _server.send(400, "text/plain", "Bad Request: Bad parameter(s)");
        return;
    }
    int startHour = start.substring(0, 2).toInt();
    int startMinute = start.substring(3, 2).toInt();
    int startSecond = start.substring(6, 2).toInt();
    time_t startTime = startHour * 3600 + startMinute * 60 + startSecond;

    int endHour = end.substring(0, 2).toInt();
    int endMinute = end.substring(3, 2).toInt();
    int endSecond = end.substring(6, 2).toInt();
    time_t endTime = endHour * 3600 + endMinute * 60 + endSecond;

    //Check if the end time is after the start time
    if(endTime <= startTime) {
        _server.send(400, "text/plain", "Bad Request: Bad parameter(s)");
        return;
    }

    _motorController.setSchedule(day, startTime, endTime);

 // success, redirect to the root page
    _server.sendHeader("Location", "/", true);
    _server.send(303);


}

void WebServer :: handleNotFound(){
  _server.send(404, "text/plain", "Not found!");
}

void WebServer::handleClient() {
    _server.handleClient();
}

void WebServer::handleRoot() {
    word machineState = _motorController.getModeActive();

    if (machineState == MODE_NOT_ALLOWED){
      _server.send(500, "text/plain","Error code: MODE_NOT_ALLOWED. Please reboot your device, if persists contact the manufacturer.");
    }

    String html = "<html><body>";
    html += "<div>";
    html += "<h2>Current state</h2>";

    switch (_motorController.getModeActive())
    {
    case MODE_MANUAL:
        html += "<p>Manual</p>";
      break;

    case MODE_SCHEDULE:
        html += "<p>Schedule</p>";
        break;

    case MODE_TIMER:
        html += "<p>Timer</p>";
        break;

    }
    
    html += "</div>";
    html += "<div>";
    html += "  <h3>Manual Control</h3>";
    html += "  <form method='get' action='manualTurnOn'>";
    html += "    <input type='submit' value='Turn On'></form>";
    html += "  <form method='get' action='manualTurnOff'>";
    html += "    <input type='submit' value='Turn Off'></form>";
    html += "</div>";

    html += "<div>";
    html += "  <h3>Timer Control</h3>";
    html += "  <form method='get' action='startTimer'>";
    html += "    <input type='submit' value='Start Timer'></form>";
    html += "</div>";

    html += "<div>";
    html += "  <h3>Schedule Control</h3>";
    html += "  <form method='get' action='schedulePeriod'>";
    html += "    <input type='submit' value='Start Schedule'></form>";
    html += "</div>";

    html += "</body></html>";

    _server.send(200, "text/html", html);
}



#endif // WEB_SERVER_H