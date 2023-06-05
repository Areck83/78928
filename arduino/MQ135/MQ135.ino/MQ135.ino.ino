#include "MQ135.h"
#include <WiFi.h>
#include <Wire.h>
#include "ThingSpeak.h"

int air_quality

const char* ssid = "xxxx"
const char* password = "x";

unsigned long myChannelNumber = x;
const char * myWriteAPIKey = "xxxxxxxx";

//Variables del tiempo
unsigned long lastTime = 0;
unsigned long timerDelay = 2000;

void setup() {
  Serial.begin(115200);
  pinMode(34, INPUT);
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);
}

void loop() {
  if( (millis()-lastTime) > timerDelay){
    if(WiFi.status() != WL_CONNECTED){
      Serial.print("Intentando la conexión");
      while(WiFi.status() != WL_CONNECTED){
        WiFi.begin(ssid, password);
        delay(5000);
      }
      Serial.println("\nConnectado.");
    }

    MQ135 gasSensor = MQ135(34); //pin del sensor
    float air_quality = gasSensor.getPPM();

    Serial.print(air_quality);
    Serial.println("PPM");
    int x = ThingSpeak.writeField(myChannelNumber, 1, air_quality, myWriteAPIKey);

    if(x == 200){
      Serial.println("Actualizacion del canal correcta.");
    }else{
      Serial.println("Hubo un problema al actualizar el canal. Error de HTTP "+String(x));
    }

    lastTime = millis();
  }
}
