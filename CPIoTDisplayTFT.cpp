
#include "CPIoTDisplayTFT.h"

CPIoTDisplayTFT::CPIoTDisplayTFT():tft(TFT_eSPI()), spr(&tft) {
  
}


void CPIoTDisplayTFT::init() {
  SPI.begin(TFT_SCLK, TFT_MISO, TFT_MOSI, TFT_CS);

  pinMode(TFT_BK, OUTPUT);
  digitalWrite(TFT_BK, HIGH);

  tft.init();
  tft.setRotation(0);
  // rotate 90 degree
  //tft.setRotation(1);
  
  tft.fillScreen(TFT_BLACK);
  tft.initDMA();
  spr.setTextFont(1);
  spr.setTextColor(TFT_WHITE);
  spr.createSprite(240, 240);
  spr.setSwapBytes(true);
  
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  //tft.loadFont(font_12); //指定tft屏幕对象载入font_12字库
  
  int x = tft.width() / 2;
  int y = tft.height() / 2 ;

  tft.drawString("武汉", tft.width() / 2, tft.height() / 2);
  tft.drawString("LilyGo Camera Plus", tft.width() / 2, tft.height() / 2 + 20);

}

void CPIoTDisplayTFT::updatePagerMessage(String sender, String receiver, String message, unsigned char* textPixels, int dataLen, int textCount) {
  Serial.println("updatePagerMessage: " + message);

  tft.fillScreen(TFT_BLACK);
  int x = tft.width() / 2;
  tft.drawString("From: " + sender, x, 20);
  tft.drawString("To: " + receiver, x, 40);
  tft.drawString(message, x, 60);

  Serial.println("dataLen: " + dataLen);
  Serial.println("textCount: " + textCount);
  /*
  unsigned char *td = (unsigned char *)malloc(sizeof(unsigned char) * dataLen * 8);

  for (int i = 0; i < dataLen; i++) {
    for (int j = 0; j < 8; j++) {
      td[i*8 + j] = (textPixels[i] >> j) & 0x01;
    }
  }
  drawTest(td, dataLen * 8, textCount);
  free(td);
  */
}

void CPIoTDisplayTFT::setStatus(String message) {
  tft.fillScreen(TFT_BLACK);
  tft.drawString(message, tft.width() / 2, tft.height() / 2 + 20);
}

void CPIoTDisplayTFT::drawTextPixel(int x, int y, int width, int height, const uint16_t* textPixels) {
  Serial.println("drawTextPixel");
  // Fill the whole sprite with black (Sprite is in memory so not visible yet)
  spr.fillSprite(TFT_BLUE);
  
  spr.setTextFont(1);
  spr.setCursor(20, 20);
  spr.println("Hello World\n");
  spr.setCursor(20, 40);
  spr.println("Hello World\n");

  spr.pushImage(x, y, width, height, (const uint16_t*)textPixels);
  
  spr.pushSprite(0, 0);
  Serial.println("drawTextPixel end");
}

void CPIoTDisplayTFT::drawTest(const unsigned char* data, int dataLen, int textCount) {
  Serial.println("drawTextPixel");
  Serial.println("dataLen: " + dataLen);
  Serial.println("textCount: " + textCount);
  
  int textDataLen = dataLen/textCount;
  uint16_t *td = (uint16_t *)malloc(sizeof(uint16_t) * textDataLen);

  for (int a = 0; a < textCount; a++) {
    for (int i = 0; i < textDataLen; i++) {
      if (data[i+ textDataLen * a] == 1) {
        td[i] = 0xFFFF;
      } else {
        td[i] = 0x0;
      }
    }
    spr.pushImage(20 + a * 30, 100, 24, 24, (const uint16_t*)td);
  }
  spr.pushSprite(0, 0);
  //drawTextPixel(20, 200, 24, 24, (const uint16_t*)td);
  free(td);
  
  Serial.println("drawTextPixel end");
}
