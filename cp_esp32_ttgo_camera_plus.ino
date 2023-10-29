// TFT Display
#include <TFT_eSPI.h>
#include <SPI.h>
//#include <SD.h>


#include "CPIoTMqtt.h"


#define TFT_BK              2

#define SDCARA_CS           0

TFT_eSPI tft = TFT_eSPI();              // Invoke library, pins defined in User_Setup.h

CPIoTMqtt mqtt;



void init_tft_display() {
  SPI.begin(TFT_SCLK, TFT_MISO, TFT_MOSI, TFT_CS);

  pinMode(TFT_BK, OUTPUT);
  digitalWrite(TFT_BK, HIGH);

  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  //tft.loadFont(font_12); //指定tft屏幕对象载入font_12字库
  
  int x = tft.width() / 2;
  int y = tft.height() / 2 ;
/*
  if (!SD.begin(SDCARA_CS)) {
      tft.drawString("SD Init Fail", x, y);
      Serial.println("SD Init Fail");
  } else {
      snprintf(buff, sizeof(buff), "SD Init Pass Type:%d Size:%lu\n", SD.cardType(), SD.cardSize() / 1024 / 1024);
      tft.drawString(buff, x, y);
      Serial.printf("totalBytes:%lu usedBytes:%lu\n", SD.totalBytes(), SD.usedBytes());
      delay(2000);
  }
*/
  tft.drawString("武汉", tft.width() / 2, tft.height() / 2);
  tft.drawString("LilyGo Camera Plus", tft.width() / 2, tft.height() / 2 + 20);

}

void update_pager_message(String sender, String receiver, String message) {
  tft.fillScreen(TFT_BLACK);
  int x = tft.width() / 2;
  tft.drawString("From: " + sender, x, 20);
  tft.drawString("To: " + receiver, x, 40);
  tft.drawString(message, x, 60);
}

void set_tft_status(String message) {
  
  tft.fillScreen(TFT_BLACK);
  
  tft.drawString(message, tft.width() / 2, tft.height() / 2 + 20);
}

void setup(){
  delay(2000);
  
  char buff[256];
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println("init display...");

  init_tft_display();

  set_tft_status("Wifi connecting...");
  mqtt.wifi_connect();

  
  set_tft_status("MQTT connecting...");
  mqtt.mqtt_connect();

  // Publish and subscribe
  mqtt.mqtt_pong();
  
  set_tft_status("MQTT subscribe...");
  mqtt.mqtt_subscribe();

  delay(1000);
/*
  tft.fillScreen(TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  */
  
  
  set_tft_status("Pager ready...");
}

void loop(){
  // clear the display
  mqtt.loop();
}
