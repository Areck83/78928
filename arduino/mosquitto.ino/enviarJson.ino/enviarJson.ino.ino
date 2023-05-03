#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#define DHTPin 15 //D15 del ESP32 DevKit

// Network
const char* ssid = "ghost";
const char* password = "12345678";
const char* mqtt_server = "192.168.137.1";

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

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

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

  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    snprintf (msg, MSG_BUFFER_SIZE, out , value);
    Serial.print("Publicando el mensaje: ");
    Serial.println(msg);
    client.publish("Testeo",out);
  }
}
