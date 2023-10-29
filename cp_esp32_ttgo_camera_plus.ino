
//#include <SD.h>

#include "CPIoTDisplayTFT.h"
#include "CPIoTMqtt.h"

#include "mqtt_const.h"



#define SDCARA_CS           0

CPIoTDisplayTFT display;
CPIoTMqtt mqtt;

void mqtt_callback_display(String text) {
  display.setStatus(text);
}

void mqtt_callback_pager_message(String sender, String receiver, String message, unsigned char* textPixels, int dataLen, int textCount) {
  display.updatePagerMessage(sender, receiver, message, textPixels, dataLen, textCount);
}

void setup(){
  delay(2000);
  
  char buff[256];
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println("init display...");

  display.init();

  delay(1000);
  
  mqtt.addDisplayCallback(&mqtt_callback_display);
  mqtt.addPagerCallback(&mqtt_callback_pager_message);
  mqtt.connect(MQTT_BROKER, MQTT_PORT);

}

void loop(){
  // clear the display
  mqtt.loop();
}
