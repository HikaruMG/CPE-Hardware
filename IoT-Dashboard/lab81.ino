#include <WiFi.h>
#include <PubSubClient.h>
#include "config.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // pixel ความกว้าง
#define SCREEN_HEIGHT 64 // pixel ความสูง 
Adafruit_SSD1306 OLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

#define RED_GPIO       42
#define YELLOW_GPIO    41
#define GREEN_GPIO     40
#define LDR_GPIO       4
#define TOPIC_LIGHT    TOPIC_PREFIX "/light"
#define TOPIC_LED_RED  TOPIC_PREFIX "/led/red"
#define TOPIC_LED_YELLOW  TOPIC_PREFIX "/led/yellow"
#define TOPIC_LED_GREEN  TOPIC_PREFIX "/led/green"
#define TOPIC_MSG TOPIC_PREFIX "/display/text"
#define TOPIC_SW TOPIC_PREFIX "/switch"
const int buttonPin = 2;
int buttonState = 0;
int laststat = 0;
int ispress = 0;
WiFiClient wifiClient;
PubSubClient mqtt(MQTT_BROKER, 1883, wifiClient);
uint32_t last_publish;


void connect_wifi() {
  printf("WiFi MAC address is %s\n", WiFi.macAddress().c_str());
  printf("Connecting to WiFi %s.\n", WIFI_SSID);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    printf(".");
    fflush(stdout);
    delay(500);
  }
  printf("\nWiFi connected.\n");
}


void connect_mqtt() {
  printf("Connecting to MQTT broker at %s.\n", MQTT_BROKER);
  if (!mqtt.connect("", MQTT_USER, MQTT_PASS)) {
    printf("Failed to connect to MQTT broker.\n");
    for (;;) {} // wait here forever
  }
  mqtt.setCallback(mqtt_callback);
  mqtt.subscribe(TOPIC_LED_RED);
  mqtt.subscribe(TOPIC_LED_YELLOW);
  mqtt.subscribe(TOPIC_LED_GREEN);
  mqtt.subscribe(TOPIC_MSG);
  mqtt.subscribe(TOPIC_SW);
  printf("MQTT broker connected.\n");
}


void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  if (strcmp(topic, TOPIC_LED_RED) == 0) {
    payload[length] = 0; // null-terminate the payload to treat it as a string
    int value = atoi((char*)payload); 
    if (value == 0) {
      digitalWrite(RED_GPIO, LOW);
    }
    else if (value == 1) {
      digitalWrite(RED_GPIO, HIGH);
    }
    else {
      printf("Invalid payload received.\n");
    }
  }
  if (strcmp(topic, TOPIC_LED_YELLOW) == 0) {
    payload[length] = 0; // null-terminate the payload to treat it as a string
    int value = atoi((char*)payload); 
    if (value == 0) {
      digitalWrite(YELLOW_GPIO, LOW);
    }
    else if (value == 1) {
      digitalWrite(YELLOW_GPIO, HIGH);
    }
    else {
      printf("Invalid payload received.\n");
    }
  }
  if (strcmp(topic, TOPIC_LED_GREEN) == 0) {
    payload[length] = 0; // null-terminate the payload to treat it as a string
    int value = atoi((char*)payload); 
    if (value == 0) {
      digitalWrite(GREEN_GPIO, LOW);
    }
    else if (value == 1) {
      digitalWrite(GREEN_GPIO, HIGH);
    }
    else {
      printf("Invalid payload received.\n");
    }
  }
  if (strcmp(topic, TOPIC_MSG) == 0) {
    payload[length] = 0; // null-terminate the payload to treat it as a string
    OLED.clearDisplay(); // ลบภาพในหน้าจอทั้งหมด
    OLED.setTextColor(WHITE, BLACK);  //กำหนดข้อความสีขาว ฉากหลังสีดำ
    OLED.setCursor(0, 0); // กำหนดตำแหน่ง x,y ที่จะแสดงผล
    OLED.setTextSize(2); // กำหนดขนาดตัวอักษร
    OLED.println((char*)payload); // แสดงผลข้อความ ALL
    OLED.display(); // สั่งให้จอแสดงผล
    delay(500);
  }
}


void setup() {
  Wire.begin(48, 47);
  pinMode(2, INPUT_PULLUP);
  pinMode(RED_GPIO, OUTPUT);
  digitalWrite(RED_GPIO, 0);
  pinMode(YELLOW_GPIO, OUTPUT);
  digitalWrite(YELLOW_GPIO, 0);
  pinMode(GREEN_GPIO, OUTPUT);
  digitalWrite(GREEN_GPIO, 0);
  connect_wifi();
  connect_mqtt();
  last_publish = 0;
  if (!OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // สั่งให้จอ OLED เริ่มทำงานที่ Address 0x3C
    Serial.println("SSD1306 allocation failed");
  } 
  else {
    Serial.println("ArdinoAll OLED Start Work !!!");
  }
}


void loop() {
  buttonState = digitalRead(buttonPin);
  //ispress != buttonState;
  // check for incoming subscribed topics
  mqtt.loop();
  

  // publish light value periodically (without using delay)
  uint32_t now = millis();
  if (now - last_publish >= 2000) {
    int level = 100 - (analogRead(LDR_GPIO)*100/4095);
    String payload(level);
    printf("Publishing light value: %d\n", level);
    mqtt.publish(TOPIC_LIGHT, payload.c_str());
    last_publish = now;
  }
  if (buttonState != laststat){
    String payload(buttonState);
    mqtt.publish(TOPIC_SW, payload.c_str());
  }
  laststat = buttonState;
 //if ((digitalRead(2)) == HIGH) {
  //  String payload(buttonState);
  //  mqtt.publish(TOPIC_SW, payload.c_str());
  //}
  //else{
   // String payload(buttonState);
  //  mqtt.publish(TOPIC_SW, payload.c_str());
  //}
}
