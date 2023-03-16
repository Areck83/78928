#include <Arduino.h>
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebSrv.h>

const char* ssid= "Issac";
const char* password = "xpe53u92";
AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);
  pinMode(2, OUTPUT);
  conectarse();
  server.on("/", HTTP_GET,[](AsyncWebServerRequest *request){
    int numParametros = request->params();
    Serial.print(numParametros);
    if(numParametros == 0){
      request -> send (200, "text/html", "<h1>hola mundo</h1>");
    }else{
      AsyncWebParameter *p = request -> getParam(0);
      String solicitud = p->value();
      if (solicitud == "on"){
        digitalWrite(2, HIGH);
        String html="se ha prendido el led";
        request -> send (200, "text/html", html);
      }
      if (solicitud == "off"){
        digitalWrite(2, LOW);
        String html="se ha apagado el led";
        request -> send (200, "text/html", html);
      }else{
        String html="<h1>hola "+ p -> value() +" desde ESP32</h1>";
        request -> send (200, "text/html", html);
      }
    }
  });
  server.on("/adios", HTTP_GET,[](AsyncWebServerRequest *r){
    r->send(200, "text/html", "<h1> adios </h1>");
  });
  server.begin();
}//Fin setup

void loop() {
  
}

void conectarse(){
    Serial.print("conectando ");
    Serial.println("ssid");
    WiFi.begin(ssid,password);
    while (WiFi.status() != WL_CONNECTED ){
      delay(500);
      Serial.print(".");
    }
    Serial.println(WiFi.localIP());
}
