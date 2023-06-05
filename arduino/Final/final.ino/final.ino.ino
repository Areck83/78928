#include <DHTesp.h>
#include "DHTesp.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <NTPClient.h> //Libreria de Taranais
#include <WiFiUdp.h>
#include "MQ135.h"//++
#define DHTpin 15 //Numero de Pin
#define PIN_MQ135 34
DHTesp dht;

//Definiciond del cliente NTP para los datos de tiempo
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

//Variables para el tiempo y fecha
String formattedDate;
String fecha;
String tiempo;

//Red
const char* ssid= "Issac";
const char* password = "xpe53u92";
const char* mqtt_server = "192.168.137.1";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

//Obtener internet de el WiFi definido previamente
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(">");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WIFI CONECTADO");
  Serial.println("Direccion IP: ");
  Serial.println(WiFi.localIP());
  timeClient.begin();
  timeClient.setTimeOffset(-21600);//Esta es la zona horaria GMT -6 MX
}

//Mostrar los mensajes del topico
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensaje recibido [");
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

//Intentar la conexion con MQTT las veces que sea posible
void reconnect() { 
  while (!client.connected()) {
    Serial.print("Intentando una conexion con MQTT...");

    //Random ID for our client
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {
      Serial.println("conectado");
      //Aqui debe de ir la ruta del tema y el elemento a enviar
      client.publish("test", "temperatura");
      client.subscribe("inTopic");
    } else {
      Serial.print("Conexion fallida, rc=");
      Serial.print(client.state());
      Serial.println(" Intentando de nuevo en 5 segundos");
      delay(5000);
    }
  }
}


void setup(){
  //++
  pinMode(34, INPUT);

  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop(){
  //++
  MQ135 gasSensor = MQ135(34); //pin del sensor
  float air_quality = gasSensor.getPPM();
  Serial.print(air_quality);
  Serial.println("PPM");

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  while(!timeClient.update()){
    timeClient.forceUpdate();
  }

  formattedDate = timeClient.getFormattedDate();
  Serial.println(formattedDate);
  
  //Separar los valores devueltos por el formattedDate
  int splitT = formattedDate.indexOf("T");
  fecha = formattedDate.substring(0,splitT);
  Serial.println(fecha);

  tiempo = formattedDate.substring(splitT+1, formattedDate.length()-1);
  Serial.println(tiempo);

  bool peligro = false;

  //Definicion del json
  char out[128];
  StaticJsonDocument<256> doc;

  //Verificar que todos los datos se estan leyendo en la consola
  //Leer siempre
  delay(dht.getMinimumSamplingPeriod()); //Establece el minimo periodo de sampleo
  float humedad =  dht.getHumidity();
  float temperatura = dht.getTemperature();
  if (isnan(humedad) || isnan(temperatura)){ //Si no es un numero
    Serial.println("No se pudo leer sensor DHT!");
    return;
  }

  //Imprimir los valores en consola
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

  int hum = getHumedad().toInt();
  int temp = getTempC().toInt();

  if((hum>15) && (temp<14)){
    peligro = true;
  }

  if (peligro == true){
    doc["esp32"] = "chuncheTest";
    doc["mensaje"] = "Posible peligro de incendio";
    doc["hora"] = tiempo;
    doc["fecha"] = fecha;
    doc["lat"] = 19;
    doc["lon"] = -92;
  }else{
    doc["esp32"] = "chuncheTest";
    doc["mensaje"] = "Valores normales";
    doc["hora"] = tiempo;
    doc["fecha"] = fecha;
    doc["lat"] = 19;
    doc["lon"] = -92;
  }

  serializeJson(doc, out);

  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    snprintf (msg, MSG_BUFFER_SIZE,out,value);
    Serial.print("Publicando el mensaje: ");
    Serial.println(msg);
    client.publish("Test",out);
  }
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
