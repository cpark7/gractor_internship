#include <SoftwareSerial.h>
#include "heltec.h"


//softwareSerial mySerial(7,6); // Arudino Uno port RX, TX
HardwareSerial mySerial(2);// UART1/Serial1 pins 16,17
#define BAND    868E6  //you can set band here directly,e.g. 868E6,915E6

void setup() {
  // for debuging 
  Serial.begin(115200);  
    Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);

  // Use software serial
  mySerial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  static int CheckFirst=0;
  static int pm_add[3][5]={0,};
  static int pm_old[3]={0,};
  int chksum=0;
  int res=0;
  unsigned char pms[32]={0,};
  
  if(mySerial.available()>=32){
    for(int j=0; j<32 ; j++){
      pms[j]=mySerial.read();
      if(j<30)
        chksum+=pms[j];
    }
    if(pms[30] != (unsigned char)(chksum>>8) 
        || pms[31]!= (unsigned char)(chksum) ){
      return;
    }
    if(pms[0]!=0x42 || pms[1]!=0x4d ){
      return;
    }

  LoRa.beginPacket();
  LoRa.setTxPower(14,RF_PACONFIG_PASELECT_PABOOST);
  LoRa.print("Dust raw data debugging :  ");
  LoRa.print("1.0ug/m3:");
  int pms1 = (pms[10]<<8)|pms[11];
  LoRa.print(pms1);
  LoRa.print("  ");
  LoRa.print("2.5ug/m3:");
  int pms25 = (pms[12]<<8)|pms[13];
  LoRa.print(pms25);

  LoRa.print("  ");
  LoRa.print("10ug/m3:");
  int pms10 = (pms[14]<<8)|pms[15];
  LoRa.print(pms10);

  
  LoRa.endPacket();

  
  } 
}
