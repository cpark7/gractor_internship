#include <SPI.h>
#include <Arduino.h>
#include "WizFi310.h"
#include "PubSubClient.h"

char ssid[] = "GRT-GUEST";
char pass[] = "77407740";

int status = WL_IDLE_STATUS;
int Relay = 3;

char server[] = "nas.gractor.com";

WiFiClient  WizFiClient;
PubSubClient client(WizFiClient);

long lastMsg = 0;
char msg[50];

void setup() {
  Serial.begin(9600);
  SPI.begin();

  setup_wifi();
  client.setServer(server, 1883);
  client.setCallback(callback);

   if (!client.connected()) {
    reconnect();
  }
  lastMsg = millis();
 
  pinMode(13, OUTPUT);         //Set Pin13 as output
  digitalWrite(13, HIGH);     //Set Pin13 High
  pinMode(Relay, OUTPUT);     //Set Pin3 as output
}


void loop() {
  client.loop();
 }

void setup_wifi() {
  Serial3.begin(115200);
  WiFi.init(&Serial3);
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  status = WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void  callback(char* topic, byte* payload, unsigned int length)
{
  Serial.print("callback");
  while ( status != WL_CONNECTED) {
    Serial.print("tlqkf");
    status = WiFi.begin(ssid, pass);
    }
    
  Serial.print("MeSsaGe arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  
   if(payload[0] == '1'){ 
     client.publish("/data/door/minki","1");
     digitalWrite(3, HIGH);
     delay(500);
     digitalWrite(3, LOW);
   }

}

void reconnect()
{
  Serial.print("reconnect");
  while ( status != WL_CONNECTED) {
    Serial.print("tlqkf");
    status = WiFi.begin(ssid, pass);
    }
  while(!client.connected())
  {
    Serial.print("Attempting MQTT connection.....");
    //Attempt to connect
    if(client.connect("WizFiClient")){
      Serial.println("connected");
      
      //Publish and subscribe
      client.subscribe("/cmd/door/#");

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
