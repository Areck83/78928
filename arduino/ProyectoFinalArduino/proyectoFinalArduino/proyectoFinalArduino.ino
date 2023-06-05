#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <DHTesp.h>
DHTesp dht;

#define DHTPin 15 //D15 del ESP32 DevKit
#define MQ135_THRESOLD_1 1000

// Configuracion de la red
const char* ssid = "ghost"; //Nombre del WiFi
const char* password = "12345678"; //Cotra del WiFi
const char* mqtt_server = "192.168.137.1"; //Conexion del MQTT
const unsigned int frecuenciaEscritura = 2500;

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

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
}

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

void reconnect() {
  
  while (!client.connected()) {
    Serial.print("Intentando una conexion MQTT...");

    //Random ID for our client
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {
      Serial.println("conectado");
      client.publish("fei/cc1/temperatura", "temperatura");
      client.subscribe("inTopic");
    } else {
      Serial.print("Conexion fallida, rc=");
      Serial.print(client.state());
      Serial.println(" Intentando de nuevo en 5 segundos");
      delay(5000);
    }
  }
}

//LED setup
void setup() {
  pinMode(2, OUTPUT);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

}

void loop() {

  //Para que se conecte al WiFi
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  //Parte del sensor de humedad
  delay(dht.getMinimumSamplingPeriod());
  float humedad = dht.getHumidity();
  float temperatura = dht.getTemperature();

  //Debugger para verificar que está leyendo el DHT
  if (isnan(humedad) || isnan(temperatura)){
    Serial.print("No se pudo leer el sensor DHT");
    return;
  }
  Serial.print(dht.getStatusString());
  Serial.print("\t");
  Serial.print(humedad,1);
  Serial.print("\t\t");
  Serial.print(temperatura,1);
  Serial.print("\t\t");
  Serial.print(dht.toFahrenheit(temperatura),1);
  Serial.print("\t\t");
  Serial.print(dht.computeHeatIndex(temperatura, humedad, false), 1);
  Serial.print("\t\t");
  Serial.println(dht.computeHeatIndex(dht.toFahrenheit(temperatura), humedad, true), 1);
  delay(2000);

  //Parte del sensor de calidad de aire MQ135
  int MQ135_info = analogRead(A0);
  if (MQ135_info < MQ135_THRESOLD_1){
    Serial.print("Aire fresquito");
  } else{
    Serial.print("Aire deplorable salte de ahi");
  }

  Serial.print(MQ135_info); //Datos analogos
  Serial.print("PPM"); //Recordar que en quimica eso es partes por millon

  //Este es el codigo del de mosquitto para enviar los datos en crudo
  //unsigned long now = millis();
  //if (now - lastMsg > 2000) {
  //  lastMsg = now;
  //  ++value;
  //  snprintf (msg, MSG_BUFFER_SIZE, "En el 402 el equipo 7 #%ld", value);
  //  Serial.print("Publicar mensaje: ");
  //  Serial.println(msg);
  //  client.publish("fei/cc1/temperatura", dtostrf(temperatura, 6, 2, msg));
  //}

  //Generacion de un JSON que se va a enviar hacia el MQTT
  char out[128];
  StaticJsonDocument<256> doc;
  doc["mensaje"] = "hola";
  doc["lat"] = 19;
  doc["long"] = -92;
  //doc["temp"] = 12;
  //JsonArray data = doc.createNestedArray("coordenadas");
  //data.add("19");
  //data.add("2.38");
  //doc["data"]=data;
  serializeJson(doc, out);

  //Codigo correspondiente a Json
  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    snprintf (msg, MSG_BUFFER_SIZE, out , value);
    Serial.print("Publicando el mensaje: ");
    Serial.println(msg);
    //Cambiar el topico de Testeo por el adecuado
    client.publish("Testeo",out);
  }
}