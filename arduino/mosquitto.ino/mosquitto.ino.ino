#include <WiFi.h>
#include <PubSubClient.h>
#include <DHTesp.h>
#define DHTPin 15 //D15 del ESP32 DevKit
DHTesp dht; 

// Update these with values suitable for your network.

const char* ssid = "Issac";
const char* password = "xpe53u92";
const char* mqtt_server = "test.mosquitto.org";
//const char* mqtt_server = "192.168.0.100";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
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
  Serial.print("Mensaje recibido [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '1') {
    digitalWrite(2, HIGH);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
  } else {
    digitalWrite(2, LOW);  // Turn the LED off by making the voltage HIGH
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Intentando una conexion MQTT...");
    // Crear un ID de cliente aleatorio
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    // Intentar conectar
    if (client.connect(clientId.c_str())) {
      Serial.println("conectado");
      // Once connected, publish an announcement...
      client.publish("test/esp32/hilox", "hola perre");
      // ... and resubscribe
      client.subscribe("inTopic");
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
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

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

  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    snprintf (msg, MSG_BUFFER_SIZE, "Esta es una prueba previa al dia final de la evaluacion #%ld", value);
    Serial.print("Publicando mensaje: ");
    Serial.println(msg);
    client.publish("test/esp32/hilox", dtostrf(temperatura, 6, 2, msg));
  }
}
