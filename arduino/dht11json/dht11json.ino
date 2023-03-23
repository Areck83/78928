#include <DHTesp.h>
#include "DHTesp.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebSrv.h>
#define DHTpin 15
DHTesp dht;

const char* ssid= "ghost";
const char* password = "12345678";
AsyncWebServer server(80);

void setup(){
  Serial.begin(115200);
  Serial.println();
  conectarse();
  Serial.println("Estado\tHumedad (%)\tTemperatura (C)\t(F)\tIndiceCalor (C)\t(F)");
  dht.setup(DHTpin, DHTesp::DHT11); //El pin que va a utilizar para los datos
  server.on("/", HTTP_GET,[](AsyncWebServerRequest *request){
    int numParametros = request->params();
    Serial.print(numParametros);
    if(numParametros == 0){
      request -> send (200, "text/html", "<h1>Bienvenido al DHT11</h1>");
    }
  });
  server.on("/hum", HTTP_GET,[](AsyncWebServerRequest *r){
    AsyncResponseStream *response = r->beginResponseStream("application/json");
    DynamicJsonDocument doc(1024);
    doc["humedad"] = getHumedad();
    serializeJson(doc, *response);
    r->send(response);
  });
  server.on("/tempc", HTTP_GET,[](AsyncWebServerRequest *r){
    AsyncResponseStream *response = r->beginResponseStream("application/json");
    DynamicJsonDocument doc(1024);
    doc["temperatura"] = getTempC();
    serializeJson(doc, *response);
    r->send(response);
  });
  server.on("/tempf", HTTP_GET,[](AsyncWebServerRequest *r){
    AsyncResponseStream *response = r->beginResponseStream("application/json");
    DynamicJsonDocument doc(1024);
    doc["temperatura"] = getTempF();
    serializeJson(doc, *response);
    r->send(response);
  });
  server.on("/calorc", HTTP_GET,[](AsyncWebServerRequest *r){
    AsyncResponseStream *response = r->beginResponseStream("application/json");
    DynamicJsonDocument doc(1024);
    doc["calor"] = getCalorC();
    serializeJson(doc, *response);
    r->send(response);
  });
  server.on("/calorf", HTTP_GET,[](AsyncWebServerRequest *r){
    AsyncResponseStream *response = r->beginResponseStream("application/json");
    DynamicJsonDocument doc(1024);
    doc["calor"] = getCalorF();
    serializeJson(doc, *response);
    r->send(response);
  });
  server.begin();
}

void loop(){
  delay(dht.getMinimumSamplingPeriod()); //Establece el minimo periodo de sampleo
  float humedad =  dht.getHumidity();
  float temperatura = dht.getTemperature();
  if (isnan(humedad) || isnan(temperatura)){ //Si no es un numero
    Serial.println("No se pudo leer sensor DHT!");
    return;
  }
  //Imprimir los valores
  Serial.print( dht.getStatusString() );
  Serial.print("\t");
  Serial.print(humedad, 1);
  Serial.print("\t\t");
  Serial.print(temperatura, 1);
  Serial.print("\t\t");
  Serial.print( dht.toFahrenheit(temperatura), 1);
  Serial.print("\t\t");
  Serial.print( dht.computeHeatIndex(temperatura, humedad, false), 1);
  Serial.print("\t\t");
  Serial.println(dht.computeHeatIndex(dht.toFahrenheit(temperatura), humedad, true), 1);
  delay(2000);
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

String getHumedad(){
  float humedad =  dht.getHumidity();
  String valor= "";
  valor.concat(humedad);
  return valor;
}

String getTempC(){
  float temperatura = dht.getTemperature();
  String tempc= "";
  tempc.concat(temperatura);
  return tempc;
}

String getTempF(){
  float temperatura = dht.toFahrenheit(temperatura);
  String tempf = "";
  tempf.concat(temperatura);
  return tempf;
}

String getCalorC(){
  float humedad =  dht.getHumidity();
  float temperatura = dht.getTemperature();
  float calor =  dht.computeHeatIndex(temperatura, humedad, false);
  String valor= "";
  valor.concat(calor);
  return valor;
}

String getCalorF(){
  float humedad =  dht.getHumidity();
  float temperatura = dht.getTemperature();
  float calor =  dht.computeHeatIndex(dht.toFahrenheit(temperatura), humedad, true);
  String valor= "";
  valor.concat(calor);
  return valor;
}




