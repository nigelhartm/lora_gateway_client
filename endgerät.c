#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>

#define ss 18
#define rst 14
#define dio0 26

String device_id = "DEVICE_ID";
int add, sub, con;

void setup() {
  add=0;
  sub=100;
  con=35;
  Serial.begin(9600);
  while(!setupLoRa()) {delay(2000);}
}

void loop() {
  LoRa.beginPacket();
  LoRa.print(device_id);
  LoRa.print(";");
  LoRa.print(add);
  LoRa.print(";");
  LoRa.print(sub);
  LoRa.print(";");
  LoRa.print(con);
  LoRa.print(";");
  LoRa.endPacket();
  add++;
  sub--;
  if(add>100) {
    add=0;
  }
  if(sub<0) {
    sub=100;
  }
  delay(5000);
}

boolean setupLoRa(){
  Serial.println("LoRa wird gestartet.");
  SPI.begin(5, 19, 27, 18);
  LoRa.setPins(ss, rst, dio0);
  for(int i=0; !LoRa.begin(868E6); i=i+1) {
    if (i==30) {
      Serial.println("LoRa starten fehlgeschlagen.");
      return false;
    }
    delay(100);
  }
  Serial.println("LoRa starten erfolgreich.");
  return true;
}
