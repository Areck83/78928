#include <DHTesp.h>
#include "DHTesp.h"
#include <Arduino.h>
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

const char* ssid= "Issac";
const char* password = "xpe53u92";
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
    String humedadWeb = "<h1> La humedad es:</h1>"+getHumedad()+"";
    r->send(200, "text/html", humedadWeb);
  });
  server.on("/tempc", HTTP_GET,[](AsyncWebServerRequest *r){
    String tempWebc = "<h1> La temperatura (Centigrados) es:</h1>"+getTempC()+"";
    r->send(200, "text/html", tempWebc);
  });
  server.on("/tempf", HTTP_GET,[](AsyncWebServerRequest *r){
    String tempwebf = "<h1> La temperatura (Fahrenheit) es: </h1>"+getTempF()+"";
    r->send(200, "text/html", tempwebf);
  });
  server.on("/calorc", HTTP_GET,[](AsyncWebServerRequest *r){
    String calorc = "<h1> El calor (Centigrados) actual es:</h1>"+getCalorC()+"";
    r->send(200, "text/html", calorc);
  });
  server.on("/calorf", HTTP_GET,[](AsyncWebServerRequest *r){
    String calorf = "<h1> El calor (Fahrenheit) actual es:</h1>"+getCalorF()+"";
    r->send(200, "text/html", calorf);
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




