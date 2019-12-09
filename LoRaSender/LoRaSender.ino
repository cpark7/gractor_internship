/* 
  Basic test program, send date at the BAND you seted.
  
  by Aaron.Lee from HelTec AutoMation, ChengDu, China
  成都惠利特自动化科技有限公司
  www.heltec.cn
  
  this project also realess in GitHub:
  https://github.com/Heltec-Aaron-Lee/WiFi_Kit_series
*/
#include "heltec.h"
#include "BLINKER_PMSX003ST.h"
HardwareSerial pmsSerial(2);// UART1/Serial1 pins 16,17

#define BAND    868E6  //you can set band here directly,e.g. 868E6,915E6

int counter = 0;

BLINKER_PMSX003ST pms;

void setup() {
  
  //WIFI Kit series V1 not support Vext control
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);
  pmsSerial.begin(9600);
  Serial2.begin(115200);
  pms.begin(pmsSerial);
    //  pms.wakeUp();
  pms.setMode(PASSIVE);
}

void loop() {
  Serial.print("Sending packet: ");
  Serial.println(counter);
  // send packet
  LoRa.beginPacket();
  LoRa.setTxPower(14,RF_PACONFIG_PASELECT_PABOOST);
 
  pms.request();
  if (pms.read())
  {
//    Serial.print("PM 1.0 (ug/m3): ");
//    Serial.println(data.PM_AE_UG_1_0);

    Serial.print("PM1.0(CF1)\t");
    Serial.print(pms.getPmCf1(1.0));
    Serial.println("ug/m3");
    Serial.print("PM2.5(CF1)\t");
    Serial.print(pms.getPmCf1(2.5));
    Serial.println("ug/m3");
    Serial.print("PM10(CF1)\t");
    Serial.print(pms.getPmCf1(10));
    Serial.println("ug/m3");
    Serial.print("PM1.0(ATO)\t");
    Serial.print(pms.getPmAto(1.0));
    Serial.println("ug/m3");
    Serial.print("PM2.5(ATO)\t");
    Serial.print(pms.getPmAto(2.5));
    Serial.print("PM10 (ug/m3): ");
    LoRa.print("test");
//    Serial.println(data.PM_AE_UG_2_5);
//
//    Serial.print("PM 10.0 (ug/m3): ");
//    Serial.println(data.PM_AE_UG_10_0);
//
//    Serial.println();
  }else
  {
    Serial.print("tlqkf");
  }
/*
* LoRa.setTxPower(txPower,RFOUT_pin);
* txPower -- 0 ~ 20
* RFOUT_pin could be RF_PACONFIG_PASELECT_PABOOST or RF_PACONFIG_PASELECT_RFO
*   - RF_PACONFIG_PASELECT_PABOOST -- LoRa single output via PABOOST, maximum output 20dBm
*   - RF_PACONFIG_PASELECT_RFO     -- LoRa single output via RFO_HF / RFO_LF, maximum output 14dBm
*/
  LoRa.print("hello ");
  LoRa.print(counter);
  LoRa.endPacket();
  
  counter++;
  digitalWrite(25, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(25, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}
