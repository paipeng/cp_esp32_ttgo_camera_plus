// TFT Display
#include <TFT_eSPI.h>
#include <SPI.h>
//#include <SD.h>
// WIFI
#include <WiFi.h>
#include <WiFiMulti.h>
// MQTT
#include <PubSubClient.h>
// JSON
#include <ArduinoJson.h>


#include "public.h"
#include "wifi_const.h"
#include "mqtt_const.h"


char DEVICE_UDID[18];



#define TFT_BK              2

#define SDCARA_CS           0

TFT_eSPI tft = TFT_eSPI();              // Invoke library, pins defined in User_Setup.h



WiFiMulti WiFiMulti;

// Use WiFiClient class to create TCP connections
WiFiClient client;
PubSubClient mqttClient(client);


//WIFI连接路由器
void wifi_connect(void)
{
  Serial.print("Connecting to ");
  delay(10);

  // We start by connecting to a WiFi network
  WiFiMulti.addAP(WIFI_SSID, WIFI_PASSWD);

  Serial.println();
  Serial.println();
  Serial.print("\nDefault ESP32 MAC Address: ");
  Serial.println(WiFi.macAddress());
  
  Serial.print("\nDevice UDID: ");
  strncpy(DEVICE_UDID, WiFi.macAddress().c_str(), 17);
  Serial.println(DEVICE_UDID);
  
  Serial.print("\n\nWaiting for WiFi... ");

  while(WiFiMulti.run() != WL_CONNECTED) {
      Serial.print(".");
      delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Connected to the Wi-Fi network");
  delay(500);
}



void mqtt_pong() {
  String mqttPong = String(MQTT_TOPIC_PREFIX) + String(MQTT_TOPIC_PONG);
  Serial.print("publish topic: " + mqttPong + "...\n");
  StaticJsonDocument<200> doc;
  doc["udid"] = String(DEVICE_UDID);
  doc["state"] = 1;
  String json;
  serializeJson(doc, json);
  
  Serial.println("pong json: ");
  Serial.print(json);
  Serial.println("");
  boolean ret = mqttClient.publish(mqttPong.c_str(), json.c_str());
  if (ret) {
    Serial.print("publish success!\n");
  } else {
    Serial.print("publish error!\n");    
  }
}

void mqtt_subscribe() {
  // udid ping
  String mqttPing = String(MQTT_TOPIC_PREFIX) + String(DEVICE_UDID) + "/" + String(MQTT_TOPIC_PING);  
  Serial.print("\nsubscribe udid ping topic: " + mqttPing + "...\n");
  boolean ret = mqttClient.subscribe(mqttPing.c_str());
  if (ret) {
    Serial.print("subscribe udid ping success!\n");
  } else {
    Serial.print("subscribe udid ping error!\n");    
  }
  // broadcasting ping
  mqttPing = String(MQTT_TOPIC_PREFIX) + String(MQTT_TOPIC_PING);  
  Serial.print("\nsubscribe broadcasting ping topic: " + mqttPing + "...\n");
  ret = mqttClient.subscribe(mqttPing.c_str());
  if (ret) {
    Serial.print("subscribe broadcasting ping success!\n");
  } else {
    Serial.print("subscribe broadcasting ping error!\n");    
  }
  // LED
  mqttPing = String(MQTT_TOPIC_PREFIX) + String(DEVICE_UDID) + "/" + String(MQTT_TOPIC_LED);  
  Serial.print("\nsubscribe led topic: " + mqttPing + "...\n");
  ret = mqttClient.subscribe(mqttPing.c_str());
  if (ret) {
    Serial.print("subscribe led success!\n");
  } else {
    Serial.print("subscribe led error!\n");    
  }

  // MESSAGE_BOARD
  mqttPing = String(MQTT_TOPIC_PREFIX) + String(DEVICE_UDID) + "/" + String(MQTT_TOPIC_MESSAGE_BOARD);  
  Serial.print("\nsubscribe message board topic: " + mqttPing + "...\n");
  ret = mqttClient.subscribe(mqttPing.c_str());
  if (ret) {
    Serial.print("subscribe message board success!\n");
  } else {
    Serial.print("subscribe message board error!\n");    
  }

  // MQTT_TOPIC_PAGER_MESSAGE
  mqttPing = String(MQTT_TOPIC_PREFIX) + String(DEVICE_UDID) + "/" + String(MQTT_TOPIC_PAGER_MESSAGE);  
  Serial.print("\nsubscribe pager message topic: " + mqttPing + "...\n");
  ret = mqttClient.subscribe(mqttPing.c_str());
  if (ret) {
    Serial.print("subscribe pager success!\n");
  } else {
    Serial.print("subscribe pager error!\n");    
  }
}


void mqtt_callback(char *topic, byte *payload, unsigned int length) {
    Serial.print("-----------------------mqtt arrived in topic: ");
    Serial.println(topic);
    Serial.print("Message:");

    String data;
    for (int i = 0; i < length; i++) {
        Serial.print((char) payload[i]);
        data += (char) payload[i];
    }
    Serial.println();
    Serial.print("Message len:");
    Serial.print(length);
    Serial.println();

    Serial.println("data: " + data);

    if (strstr(topic, "PING")) {
      mqtt_pong();
      
    } else if (strstr(topic, "LED")) {
      StaticJsonDocument<200> doc;
      DeserializationError error = deserializeJson(doc, data);
    
      // Test if parsing succeeds.
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
      } else {
        long state = doc["state"];
        //gpio_led_toggle(state);
      }
    } else if (strstr(topic, "MESSAGE_BOARD")) {
      StaticJsonDocument<200> doc;
      DeserializationError error = deserializeJson(doc, data);
    
      // Test if parsing succeeds.
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
      } else {
        String message = doc["message"];
        Serial.println(message);
        //oled_message_board(message);
      }
    } else if (strstr(topic, MQTT_TOPIC_PAGER_MESSAGE)) {
      StaticJsonDocument<2000> doc;
      //DynamicJsonDocument doc(ESP.getMaxAllocHeap());
      DeserializationError error = deserializeJson(doc, data);
    
      // Test if parsing succeeds.
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
      } else {
        String message = doc["message"];
        String sender = doc["sender"];
        String receiver = doc["receiver"];
        
        Serial.println(message);
        update_pager_message(sender, receiver, message);
      }
    }
    Serial.println();
    Serial.println("-----------------------mqtt-end");
}


void mqtt_connect() {
  //connecting to a mqtt broker
  mqttClient.setServer(MQTT_BROKER, MQTT_PORT);
  mqttClient.setCallback(mqtt_callback);
  mqttClient.setBufferSize(5120);
  mqttClient.setKeepAlive(60*60);
  while (!mqttClient.connected()) {
      String client_id = "cp-esp32-client-";
      client_id += String(WiFi.macAddress());
      Serial.printf("The client %s connects to the public MQTT broker\n", client_id.c_str());
      if (mqttClient.connect(client_id.c_str(), MQTT_USERNAME, MQTT_PASSWORD)) {
          Serial.println("Public EMQX MQTT broker connected");
          //gpio_led_mqtt_toggle(1);
      } else {
          Serial.print("failed with state ");
          Serial.print(mqttClient.state());  
          Serial.println("\n");
          //gpio_led_mqtt_toggle(0);
          delay(2000);
      }
  }    
}

void mqtt_publish_received(float temperature) {
  String mqttTopic = String(MQTT_TOPIC_PREFIX) + String(MQTT_TOPIC_TEMPERATURE);
  Serial.print("publish topic: " + mqttTopic + "...\n");
  String data = "{\"udid\": \"" + String(DEVICE_UDID) + "\", \"state\": 1, \"value\": " + temperature + "}";
  boolean ret = mqttClient.publish(mqttTopic.c_str(), data.c_str());
  if (ret) {
    Serial.print("publish temperature success!\n");
  } else {
    Serial.print("publish temperature error!\n");    
  }
}

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
  wifi_connect();

  
  set_tft_status("MQTT connecting...");
  mqtt_connect();

  // Publish and subscribe
  mqtt_pong();
  
  set_tft_status("MQTT subscribe...");
  mqtt_subscribe();

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
  
  if (mqttClient.connected()) {
    mqttClient.loop();
  } else {
    Serial.println("mqtt disconnected");    
  }
}
