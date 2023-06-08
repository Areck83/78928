#include <ESP32Time.h>
//Este codigo configura la fecha y hora en el ESP32 para poder mostrarla posteriormente sin necesidad de un servidor NTP
ESP32Time rtc(3600);  // GMT+1. No es necesario editar

void setup() {
  Serial.begin(115200);
  rtc.setTime(00, 06, 18, 7, 6, 2023);
  //Se supone que arriba le puse que son las 7 de la tarde del 7 de junio de 2023
  //Aunque diga 18, porque al parecer los gmt siguen desordenados. 
}

void loop() {
  Serial.println(rtc.getTime());
  String dia = String(rtc.getDay());
  String mes = String(rtc.getMonth());
  String anio = String(rtc.getYear());
  Serial.println(dia+"/"+mes+"/"+anio);
  delay(1000);
}
