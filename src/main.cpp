#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ctype.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <./controllers/poolMotorController.h>
#include <./controllers/webServerController.h>
#include <./credentials.h>

#define UPDATENTC 1000

PoolMotorController poolMotor(D0);
WebServer webServer(ESP_HOST, WIFI_SSID, WIFI_PASSWORD, 80, poolMotor);
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

unsigned long currentMillis  = millis();
unsigned long updateTime = currentMillis + UPDATENTC;

void setup() {

  webServer.begin();
  if(webServer.wifiStatus()){
      timeClient.begin();
      timeClient.setTimeOffset(-3);
  }
// test
  
}
void loop() {
    if(webServer.wifiStatus()){

    webServer.handleClient();
    timeClient.update();

    unsigned long currentMillis = millis();
    if (currentMillis >= updateTime) {
        time_t currentTime = timeClient.getEpochTime();
        switch (poolMotor.getModeActive()) {
            case MODE_TIMER:
                poolMotor.checkTimer();
                break;
            case MODE_SCHEDULE:
                poolMotor.checkSchedule(currentTime);
                break;
        }
        updateTime = currentMillis + UPDATENTC;
    }

    }else{
        Serial.println("Connection lost, trying to reconnect in 1 minute");
        delay(60000);
        webServer.begin();
        timeClient.begin();
        timeClient.setTimeOffset(-3);
    }

}