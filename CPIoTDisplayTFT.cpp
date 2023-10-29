
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

void CPIoTDisplayTFT::updatePagerMessage(String sender, String receiver, String message, unsigned char* textPixels, int dataLen) {
  tft.fillScreen(TFT_BLACK);
  int x = tft.width() / 2;
  tft.drawString("From: " + sender, x, 20);
  tft.drawString("To: " + receiver, x, 40);
  tft.drawString(message, x, 60);


  int fontWidth = sqrt(dataLen);

  unsigned char td[576];
  for (int i = 0; i < 576; i++) {
    td[i] = ((i%2) << 8);
  }
  drawTextPixel(20, 200, 24, 24, td);
}

void CPIoTDisplayTFT::setStatus(String message) {
  tft.fillScreen(TFT_BLACK);
  tft.drawString(message, tft.width() / 2, tft.height() / 2 + 20);
}

void CPIoTDisplayTFT::drawTextPixel(int x, int y, int width, int height, unsigned char* textPixels) {
  TFT_eSprite spr = TFT_eSprite(&tft);

  spr.setColorDepth(8);
  spr.createSprite(width,height);
  spr.setSwapBytes(true);
  
  spr.pushImage(x, y, width,height, (uint16_t *)textPixels, TFT_WHITE);
  spr.pushSprite(0, 0, 0);

  // Fill the whole sprite with black (Sprite is in memory so not visible yet)
  spr.fillSprite(TFT_BLACK);
  
  // Draw a blue rectangle in sprite so when we move it 1 pixel it does not leave a trail
  // on the blue screen background
  spr.drawRect(0, 0, 120, 120, TFT_BLUE);
}

void CPIoTDisplayTFT::drawTest() {
  unsigned char td[576];
  for (int i = 0; i < 576; i++) {
    td[i] = ((i%2) << 8);
  }
  drawTextPixel(20, 200, 24, 24, td);
}
