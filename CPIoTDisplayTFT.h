#ifndef CPIOT_DISPLAY_TFT_H
#define CPIOT_DISPLAY_TFT_H
#include <Arduino.h>

// TFT Display
#include <TFT_eSPI.h>
#include <SPI.h>


#define TFT_BK              2

class CPIoTDisplayTFT {
  public:
    CPIoTDisplayTFT();
    void init();
    void setStatus(String text);
    void updatePagerMessage(String sender, String receiver, String message, unsigned char* textPixels, int dataLen);
    void drawTextPixel(int x, int y, int width, int height, const uint16_t* textPixels);

    void drawTest(const unsigned char* data, int dataLen, int textCount);
    
  private:
    TFT_eSPI tft;// = TFT_eSPI();              // Invoke library, pins defined in User_Setup.h
    TFT_eSprite spr;

};
#endif
