#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "ESP8266UpdatePack.h"
#include "FS.h"

#define led_onboard 2 /* ESP-12 led on board connected to pin 2 */

const char* ssid = "wifi_ssid";
const char* password = "wifi_password";
const char* username = "admin";
const char* userpassword = "";

ESP8266UpdatePack hupdate;
ESP8266WebServer server(80);

void setup(void){
  SPIFFS.begin(); //If use SPIFFS files
  
  #ifdef led_onboard
    pinMode(led_onboard, OUTPUT);
    digitalWrite(led_onboard,HIGH);
  #endif
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    #ifdef led_onboard
      digitalWrite(led_onboard,!digitalRead(led_onboard));
    #endif
    delay(500);
  }

  server.onNotFound(handleNotFound);
  hupdate.set(&server,"/pkgupdate",username,userpassword); //Setup Update Server
  server.begin();
}

void loop(void){
  server.handleClient();
}

void handleNotFound(){
  server.send(404, "text/plain", "404, FileNotFound.");
}

