
#include <TFT_eSPI.h>
#include <SPI.h>
#include "fonts/PuHuiTi8.h"
//#include <SD.h>



#define TFT_BK              2

#define SDCARA_CS           0

TFT_eSPI tft = TFT_eSPI();              // Invoke library, pins defined in User_Setup.h


void setup(){
  char buff[256];
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println("init display...");
  

  SPI.begin(TFT_SCLK, TFT_MISO, TFT_MOSI, TFT_CS);

  pinMode(TFT_BK, OUTPUT);
  digitalWrite(TFT_BK, HIGH);

  tft.init();
  tft.setRotation(0);
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.loadFont(PuHuiTi8); //指定tft屏幕对象载入font_12字库
  
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


  delay(1000);
/*
  tft.fillScreen(TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  */
}

void loop(){
  // clear the display
  
  tft.drawString("TFT_eSPI 武汉", tft.width() / 2, tft.height() / 2);
  tft.drawString("LilyGo Camera Plus", tft.width() / 2, tft.height() / 2 + 20);
  delay(1000);
}
