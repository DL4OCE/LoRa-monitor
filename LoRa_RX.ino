//Libraries for LoRa
#include <SPI.h>
#include <LoRa.h>

// OLED SSD1306
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// time sync
#include <WiFi.h>
#include "time.h"

const char* ssid     = "";
const char* password = "";

//const char* ntpServer = "de.pool.ntp.org";
const char* ntpServer = "192.168.1.1";
// settings for .DE
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 3600;

// define the pins used by the LoRa transceiver module
// these can be different on different devices
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 23 // 14
#define DIO0 26

// 433E6 for Asia, 866E6 for Europe, 915E6 for North America
#define BAND 866E6

// OLED pins
// these can be different on different devices
#define OLED_SDA 21 //4
#define OLED_SCL 22 // 15 
#define OLED_RST 23 // 16
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);

String LoRaData, tmpData;
int ReceivedPackets = 0;

void setup() { 
  Serial.begin(115200);
  Serial.println("========== LoRa RX ==========");

  //reset OLED display via software
  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, LOW);
  delay(20);
  digitalWrite(OLED_RST, HIGH);
  
  //initialize OLED
  Wire.begin(OLED_SDA, OLED_SCL);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c, false, false)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("WiFi: ");

  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.print(ssid);
  display.print(ssid);
  display.display();
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("\nWiFi connected, IP: ");
  Serial.println(WiFi.localIP());
  display.setCursor(0,10);
  display.print("IP: ");
  display.print(WiFi.localIP());
  display.display();

  // Init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Serial.println(printLocalTime());
  display.setCursor(0,20);
  display.print("LoRa RX Init OK");
  display.setCursor(0,30);
  display.print("Boottime: ");
  display.setCursor(78,30);
  display.print(printLocalTime());
  display.setCursor(0,40);
  display.print("Receiving...");
  display.display();
  
  SPI.begin(SCK, MISO, MOSI, SS);
  //setup LoRa transceiver module
  LoRa.setPins(SS, RST, DIO0);

  if (!LoRa.begin(BAND)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("Init OK");
  display.display();  
}

void loop() {
  //try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    ReceivedPackets++;
    Serial.print("Received packet ");
    LoRaData = "";
    while (LoRa.available()) {
      LoRaData += LoRa.readString();
    }
    Serial.print(LoRaData);

    int rssi = LoRa.packetRssi();
    Serial.print("\nwith RSSI ");    
    Serial.println(rssi);

    display.clearDisplay();
    display.setCursor(0,0);
    display.print("#");
    display.setCursor(6,0);
    display.print(ReceivedPackets);
    display.setCursor(36,0);
    display.print(rssi);
    display.setCursor(60,0);
    display.print("dB");
    display.setCursor(78,0);
    display.print(printLocalTime());    
    display.setCursor(0,10);
    display.print(LoRaData);
    display.display();   
  }
}

String printLocalTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    return("--:--:--");
  } else {
    char strTime[9];
    strftime(strTime, sizeof strTime, "%H:%M:%S", &timeinfo);
    return(strTime);
  }

}

String old_printLocalTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
//    Serial.println("Failed to obtain time");
    return(" no time");
  }
}

