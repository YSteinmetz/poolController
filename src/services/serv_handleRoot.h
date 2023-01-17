#ifndef SERV_HANDLE_ROOT_H
#define SERV_HANDLE_ROOT_H

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

class serv_handleRoot
{
private:
public:
    serv_handleRoot();
    String begin();
};

String serv_handleRoot::begin(){

word machineState = _motorController.getModeActive();

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

}



#endif