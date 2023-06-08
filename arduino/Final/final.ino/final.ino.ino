//Test pre final. Este codigo lee los valores del DHT11.
//Se conecta y suscribe al broker de mosquito en el topico especificado. 
//Se configura la hora al ESP32 manualmente y se toman esos valores.
//Se envia un JSON de reporte de prueba para el proyecto final. 

//Librerias
#include <WiFi.h>
#include <PubSubClient.h>
#include <DHTesp.h>
#include <ArduinoJson.h>
#include <ESP32Time.h>
#include "MQ135.h"

#define DHTPin 15 //D15 del ESP32 DevKit
ESP32Time rtc(3600); 
DHTesp dht; 

const int mq135pin = 26;

//Red
const char* ssid = "Issac";
const char* password = "xpe53u92";
const char* mqtt_server = "test.mosquitto.org"; //Broker mosquito

//Config para mosquitto
WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

//Wifi
void setup_wifi() {

  delay(10);
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connectado");
  Serial.println("Direccion IP: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensaje recibido en el topico [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  if ((char)payload[0] == '1') {
    digitalWrite(2, HIGH);
  } else {
    digitalWrite(2, LOW);
  }

}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Intentando una conexion MQTT...");
    // Crear un ID de cliente aleatorio
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    // Intentar conectar
    if (client.connect(clientId.c_str())) {
      Serial.println("conectado");
      client.subscribe("test/esp32/hilox");
      Serial.println("Suscrito al topico indicado");
    } else {
      Serial.print("Conexion fallida, rc=");
      Serial.print(client.state());
      Serial.println(" Intentando de nuevo en 5 segundos");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(2, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  dht.setup(DHTPin, DHTesp::DHT11); //Configuracion de pin de datos
  rtc.setTime(00, 57, 19, 7, 6, 2023);
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  bool peligro;
  int sensorValue = analogRead(mq135pin);

  String hora = rtc.getTime();
  String dia = String(rtc.getDay());
  String mes = String(rtc.getMonth());
  String anio = String(rtc.getYear());
  String fecha = dia+"/"+mes+"/"+anio;

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  delay(dht.getMinimumSamplingPeriod());

  float humedad = dht.getHumidity();
  float temperatura = dht.getTemperature();

  if (isnan(humedad) || isnan(temperatura)){
    Serial.print("No se pudo leer el sensor DHT");
    return;
  }

  //Este codigo es agregado para el dht11 
  Serial.print( dht.getStatusString() );
  Serial.print("\t");
  Serial.print("humedad:");
  Serial.print(humedad, 1);
  Serial.print("\t\t");
  Serial.print("C:");
  Serial.print(temperatura, 1);
  Serial.print("\t\t");
  Serial.print("FH:");
  Serial.print( dht.toFahrenheit(temperatura), 1);
  Serial.print("\t\t");
  Serial.print("IndiceCalor:");
  Serial.print( dht.computeHeatIndex(temperatura, humedad, false), 1);
  Serial.print("\t\t");
  Serial.print("IndiceCalorF:");
  Serial.println(dht.computeHeatIndex(dht.toFahrenheit(temperatura), humedad, true), 1);
  delay(30000);

  //Definicion del JSON
  char reporte[128];
  StaticJsonDocument<256> mensaje;
  if ( (getHumedad()<10) && (getTempC()>45)){
    mensaje["ESP32"]="Test1"; 
    mensaje["mensaje"]="Posible riesgo de incendio";
    mensaje["hora"]=hora;
    mensaje["fecha"]=fecha;
    mensaje["lat"]="19";
    mensaje["lon"]="-96";
  }else{
    mensaje["ESP32"]="Test1";
    mensaje["mensaje"]="Valores normales";
    mensaje["hora"]=hora;
    mensaje["fecha"]=fecha;
    mensaje["lat"]="19";
    mensaje["lon"]="-96";
  }

  serializeJson(mensaje,reporte);

  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    snprintf (msg, MSG_BUFFER_SIZE, "Este texto es de prueba#%ld", value);
    Serial.print("Publicando mensaje: ");
    Serial.println(msg);
    client.publish("test/esp32/hilox", reporte);
  }
}

float getHumedad(){
  float humedad =  dht.getHumidity();
  return humedad;
}

float getTempC(){
  float temperatura = dht.getTemperature();
  return temperatura;
}

float getTempF(){
  float temperatura = dht.toFahrenheit(temperatura);
  return temperatura;
}

float getCalorC(){
  float humedad =  dht.getHumidity();
  float temperatura = dht.getTemperature();
  float calor =  dht.computeHeatIndex(temperatura, humedad, false);
  return calor;
}

float getCalorF(){
  float humedad =  dht.getHumidity();
  float temperatura = dht.getTemperature();
  float calor =  dht.computeHeatIndex(dht.toFahrenheit(temperatura), humedad, true);
  return calor;
}
