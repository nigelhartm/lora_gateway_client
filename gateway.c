#include <SPI.h>
#include <LoRa.h>
#include <WiFi.h>

#define ss 18
#define rst 14
#define dio0 26

const char* ssid     = "SSID";
const char* password = "PASSWORD";
const char* server   = "SERVER_IP";
String gateway_id    = "GATEWAY_ID"; 
WiFiClient client;
String RxString;
int RxRSSI;
boolean rx_fp;
long lastrx;

void setup() {
  Serial.begin(9600);
  while(!setupWiFi()){delay(2000);}
  while(!setupLoRa()){delay(2000);}
}

void loop() {
  if(checkLoRaData()) {
    if(checkWiFiConnection()) {
      forwardData();
    }
  }
}

boolean setupWiFi(){
  Serial.println("WiFi wird gestartet.");
  WiFi.begin(ssid, password);
  for(int i=0; WiFi.status() != WL_CONNECTED; i=i+1) {
    if (i==30) {
      Serial.println("WiFi starten fehlgeschlagen.");
      return false;
    }
    delay(100);
  }
  Serial.println("WiFi starten erfolgreich.");
  return true;
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

boolean checkLoRaData() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    Serial.print("LoRaIncoming: ");
    boolean clr = 1;
    rx_fp = true;
    while (LoRa.available()) {
      if (clr == 1) {
        RxString = "";
        clr = 0;
      }
      RxString += (char)LoRa.read();
    }
    RxRSSI = LoRa.packetRssi();
    Serial.println(RxString);
    lastrx = millis() / 1000;
    clr = 1;
    return true;
  }
  return false;
}

boolean checkWiFiConnection() {
  if(WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi angehalten. Versuche erneut zu verbinden.");
    WiFi.disconnect();
    WiFi.mode(WIFI_OFF);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    delay(5000);
    if(WiFi.status() != WL_CONNECTED) {
      Serial.println("Gateway wird neugestartet.");
      ESP.restart();
    }
  }
  return true;
}

void forwardData() {
  Serial.println("Verbinde mit IoT-Plattform auf Port 8080.");
  if (!client.connect(server, 8080)) {
    Serial.println("Fehlgeschlagen!");
  }
  else {
    int  ind1 = RxString.indexOf(';');
    String device_id = RxString.substring(0, ind1);
    int  ind2 = RxString.indexOf(';', ind1+1 );
    String key1 = RxString.substring(ind1+1, ind2);
    int ind3 = RxString.indexOf(';', ind2+1 );
    String key2 = RxString.substring(ind2+1, ind3);
    int ind4 = RxString.indexOf(';', ind3+1 );
    String key3 = RxString.substring(ind3+1,ind4);
    Serial.println("IoT-Plattform verbindung hergestellt.");
    String x = "POST /api/v1/" + device_id + "/telemetry HTTP/1.1";
    client.println(x);
    client.println("Content-Type: text/plain");
    client.print("User-Agent: ");
    client.println(gateway_id);
    client.println("Accept: */*");
    client.println("Cache-Control: no-cache");
    client.println("Host: 192.168.178.38:8080");
    client.println("Accept-Encoding: gzip, deflate, br");
    client.println("Connection: keep-alive");
    client.print("Content-Length: ");
    client.println(RxString.length()+9);
    client.println();
    client.print("{\"key1\":\"");
    client.print(key1);
    client.print("\", \"key2\":\"");
    client.print(key2);
    client.print("\", \"key3\":\"");
    client.print(key3);
    client.println("\"}");
    delay(50);
    Serial.println("POST /api/v1/" + device_id + "/telemetry HTTP/1.1");
    Serial.println("Content-Type: text/plain");
    Serial.print("User-Agent: ");
    Serial.println(gateway_id);
    Serial.println("Accept: */*");
    Serial.println("Cache-Control: no-cache");
    Serial.println("Host: 192.168.178.38:8080");
    Serial.println("Accept-Encoding: gzip, deflate, br");
    Serial.println("Connection: keep-alive");
    Serial.print("Content-Length: ");
    Serial.println(RxString.length()+9);
    Serial.println();
    Serial.print("{\"key1\":\"");
    Serial.print(key1);
    Serial.print("\", \"key2\":\"");
    Serial.print(key2);
    Serial.print("\", \"key3\":\"");
    Serial.print(key3);
    Serial.println("\"}");
    Serial.println("Warte auf Antwort.");
    int res_timeout = 0;
    while (!client.available()) {
      delay(1000);
      if (res_timeout++ >= 30) {
        client.stop();
        Serial.println();
        Serial.println("IoT-Plattform antwortet nicht.");
        break;
      }
    }
    Serial.println("Server-Antwort: ");
    while (client.available()) {
      char c = client.read();
      Serial.write(c);
    }
    Serial.println();
    Serial.println();
    if (!client.connected()) {
      Serial.println("IoT-Plattform verbindung beendet.");
      client.stop();
    }
  }
}
