#include <SoftwareSerial.h>
#include <MHZ19PWM.h>   
#include "SNIPE.h"
#include "DHT.h"

#define DHTPIN 2

#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
MHZ19PWM mhz(3, MHZ_DELAYED_MODE);

#define PING  1
#define PONG  2

#define CODE  PONG    /* Please define PING or PONG */

#define TXpin 11
#define RXpin 10
#define ATSerial Serial

//16byte hex key
String lora_app_key = "11 22 33 44 55 66 77 88 99 aa bb cc dd ee ff 00";  

SoftwareSerial DebugSerial(RXpin,TXpin);
SNIPE SNIPE(ATSerial);

void setup() {
  ATSerial.begin(115200);

  // put your setup code here, to run once:
  while(ATSerial.read()>= 0) {}
  while(!ATSerial);

  DebugSerial.begin(115200);

  /* SNIPE LoRa Initialization */
  if (!SNIPE.lora_init()) {
    DebugSerial.println("SNIPE LoRa Initialization Fail!");
    while (1);
  }

  /* SNIPE LoRa Set Appkey */
  if (!SNIPE.lora_setAppKey(lora_app_key)) {
    DebugSerial.println("SNIPE LoRa app key value has not been changed");
  }
  
  /* SNIPE LoRa Set Frequency */
  if (!SNIPE.lora_setFreq(LORA_CH_1)) {
    DebugSerial.println("SNIPE LoRa Frequency value has not been changed");
  }

  /* SNIPE LoRa Set Spreading Factor */
  if (!SNIPE.lora_setSf(LORA_SF_12)) {
    DebugSerial.println("SNIPE LoRa Sf value has not been changed");
  }

  /* SNIPE LoRa Set Rx Timeout 
   * If you select LORA_SF_12, 
   * RX Timout use a value greater than 5000  
  */
  if (!SNIPE.lora_setRxtout(15000)) {
    DebugSerial.println("SNIPE LoRa Rx Timout value has not been changed");
  }  
  if (!SNIPE.lora_setTxtout(2000)) {
    DebugSerial.println("SNIPE LoRa Rx Timout value has not been changed");
  }  
    
  DebugSerial.println("SNIPE LoRa PingPong Test");

  mhz.useLimit(5000);
  
}


void loop() {
  
  
  float co2 = mhz.getCO2();
  int co2_ = co2;
  int h = dht.readHumidity();
  int t = dht.readTemperature();
  int lux = analogRead(A0);      
  String h_ = String(h);
  String t_ = String(t);
  String lux_ = String(lux);
  String co2__ = String(co2_);
  while(h_.length() < 2){
    h_ = "0"+h_;
  }
  while(t_.length() < 2){
    t_ = "0"+t_;
  }
  while(lux_.length() < 4){
    lux_ = "0"+lux_;
  }
  while(co2__.length()<5){
    co2__ = "0"+co2__;
  }
  String total = "1_" + h_ + t_ + lux_+co2__;

  
#if CODE == PONG
        String ver = SNIPE.lora_recv();
        delay(300);

        DebugSerial.println(ver);
        
        if (ver == "PING" )
        {
          DebugSerial.println("recv success");
          DebugSerial.print("RSSI_");
          DebugSerial.println(SNIPE.lora_getRssi());
          DebugSerial.print("SNR_");
          DebugSerial.println(SNIPE.lora_getSnr());

          if(SNIPE.lora_send(total))
          {
            DebugSerial.println("Total_send success");
          }
          else
          {
            DebugSerial.println("Total_send fail");
            delay(500);
          }
          }
   

#endif
}
