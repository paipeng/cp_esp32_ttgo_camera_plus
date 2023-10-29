
#include "CPIoTDisplayTFT.h"

CPIoTDisplayTFT::CPIoTDisplayTFT():tft(TFT_eSPI()) {
  
}


void CPIoTDisplayTFT::init() {
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

void CPIoTDisplayTFT::updatePagerMessage(String sender, String receiver, String message) {
  tft.fillScreen(TFT_BLACK);
  int x = tft.width() / 2;
  tft.drawString("From: " + sender, x, 20);
  tft.drawString("To: " + receiver, x, 40);
  tft.drawString(message, x, 60);
}

void CPIoTDisplayTFT::setStatus(String message) {
  
  tft.fillScreen(TFT_BLACK);
  
  tft.drawString(message, tft.width() / 2, tft.height() / 2 + 20);
}
