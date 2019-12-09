#include "heltec.h"
#include <WiFi.h>
#include <SPI.h>
#include "PubSubClient.h"

#define BAND    868E6  //you can set band here directly,e.g. 868E6,915E6

char msg[100];
char server[] = "nas.gractor.com";
const char* ssid     = "GRT-GUEST";
const char* password = "77407740";
WiFiClient WizFiClient;
PubSubClient client(WizFiClient);

void setup() {
    //WIFI Kit series V1 not support Vext control
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
 Serial.begin(9600);
  Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

     client.setServer(server, 1883);
  client.setCallback(callback);
  if (!client.connected()) {
    reconnect();
  }
  
}

void loop() {
  // try to parse packet
  
  int i = 0;
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // received a packet
 //   Serial.print("Received packet '");

    // read packet
    while (LoRa.available()) {
     msg[i] = (char)LoRa.read();
     i++;
    
    }
   Serial.print(msg);

    // print RSSI of packet
//    Serial.print("' with RSSI ");
//    Serial.println(LoRa.packetRssi());
  }
  client.loop();
}

void  callback(char* topic, byte* payload, unsigned int length)
{
  Serial.print("MeSsaGe arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  client.publish("/data/pms/mj",msg);

  if((char)payload[0] == '1')
  {
    digitalWrite(13, LOW);
  }
  else
  {
    digitalWrite(13, HIGH);
  }  
}

void reconnect()
{
  while(!client.connected())
  {
    Serial.print("Attempting MQTT connection.....");
    //Attempt to connect
    if(client.connect("WizFiClient")){
      Serial.println("connected");
      
      //Publish and subscribe
//      client.subscribe("/data/#");
    }
    else
    {
      Serial.println(client.connected());
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);      
    }
  }
}
