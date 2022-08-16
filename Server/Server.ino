#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include "SNIPE.h"

#define PING  1
#define PONG  2

#define CODE  PING    /* Please define PING or PONG */

#define TXpin 11
#define RXpin 10
#define ATSerial Serial



//16byte hex key
String lora_app_key = "11 22 33 44 55 66 77 88 99 aa bb cc dd ee ff 00";  

SoftwareSerial DebugSerial(RXpin,TXpin);
SNIPE SNIPE(ATSerial);
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  lcd.begin();
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
  if (!SNIPE.lora_setRxtout(5000)) {
    DebugSerial.println("SNIPE LoRa Rx Timout value has not been changed");
  }  
    
  DebugSerial.println("SNIPE LoRa PingPong Test");
}


void loop() {

#if CODE == PING
        if (SNIPE.lora_send("PING"))
        {
          DebugSerial.println("send success");
          
          String ver = SNIPE.lora_recv();
          DebugSerial.println(ver);
          int ver1 = ver.charAt(0);
          
          if (ver1 ==49)
          {
            DebugSerial.println("recv success");
            DebugSerial.print("RSSI_");
            DebugSerial.println(SNIPE.lora_getRssi());
            DebugSerial.print("SNR_");
            DebugSerial.println(SNIPE.lora_getSnr());    
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("T : ");
            lcd.setCursor(4, 0);
            lcd.write(ver.charAt(4));
            lcd.write(ver.charAt(5));
            lcd.setCursor(8, 0);
            lcd.print("H : ");
            lcd.setCursor(12, 0);
            lcd.write(ver.charAt(2)); 
            lcd.write(ver.charAt(3));
            lcd.setCursor(0, 1);
            lcd.print("L : ");
            lcd.setCursor(4, 1);
     
            lcd.write(ver.charAt(7));
            lcd.write(ver.charAt(8));
            lcd.write(ver.charAt(9));
            lcd.setCursor(8,1);
            lcd.print("P : ");
            lcd.setCursor(12,1);
            lcd.write(ver.charAt(11));
            lcd.write(ver.charAt(12));
            lcd.write(ver.charAt(13));
            lcd.write(ver.charAt(14));

            
            
            
          }
          else
          {
            DebugSerial.println("recv fail");
            lcd.clear();
            lcd.setCursor(6, 0);
            lcd.print("DATA");
            lcd.setCursor(6, 1);
            lcd.print("RECV...");
          }
        }


       
#endif
}
