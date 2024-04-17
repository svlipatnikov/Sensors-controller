#include <microDS18B20.h>
#define DS1_PIN D2
#define DS2_PIN D4
#define DS3_PIN D0

uint8_t ds1_addr[][8] = {
  { 0x28, 0x0F, 0x51, 0x16, 0xA8, 0x01, 0x3C, 0x59 }, // kit
  { 0x28, 0x36, 0x74, 0x16, 0xA8, 0x01, 0x3C, 0x08 }, // bed
  { 0x28, 0x3C, 0xA6, 0x16, 0xA8, 0x01, 0x3C, 0x1A }, // child
  { 0x28, 0x02, 0x74, 0x16, 0xA8, 0x01, 0x3C, 0x39 }, // bath
};

MicroDS18B20<DS1_PIN, DS_ADDR_MODE, 4> ds1; 


#include "DHT.h"
#define DHT1_PIN D7
#define DHT2_PIN D6
#define DHT3_PIN D5
#define DHT4_PIN D1

DHT dht1(DHT1_PIN, DHT22);
DHT dht2(DHT2_PIN, DHT22);
DHT dht3(DHT3_PIN, DHT22);
DHT dht4(DHT4_PIN, DHT22);


#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
ESP8266WebServer httpServer(80);
ESP8266HTTPUpdateServer httpUpdater;


#include <PubSubClient.h>
WiFiClient NodeMCU_sensors;
PubSubClient mqttClient(NodeMCU_sensors);


unsigned long handlerTime = millis();
int HANDLER_PERIOD = 30 * 1000; // 30c


void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  // DS
  ds1.setAddress((uint8_t*)ds1_addr);
  ds1.setResolutionAll(10); // Установить разрешение 9-12 бит у всех датчиков на линии

  // DHT
  dht1.begin();
  dht2.begin();
  dht3.begin();
  dht4.begin();

  // подключение к Wi-Fi
  ConnectWiFi();
  ArduinoOTA.begin();

  // подключение к MQTT
  MqttInit();
}


void loop() {
  // для обновления по воздуху
  ArduinoOTA.handle();
  httpServer.handleClient();   

  // для функций MQTT
  MqttHandler();  

  // проверка подключений к wifi и серверам
  CheckWiFiConnection();

  // считывание и отправка данных с датчиков
  SensorsHandler();
}


void SensorsHandler(void) {
  if ((long)millis() - handlerTime > HANDLER_PERIOD) {
    handlerTime = (long)millis(); 

    digitalWrite(LED_BUILTIN, LOW);
    readAllDS1();
    readAllDHT();
    digitalWrite(LED_BUILTIN, HIGH);
  } 
}

void readAllDS1() {
  for (int i = 0; i < sizeof(ds1_addr) / 8; i++) {
    float temp = ds1.getTemp(i);
    String temp_s = ds1.readTemp(i) ? String(ds1.getTemp(i)) : "null";
    String name = "ds1_" + String(i);
    mqttSend(name, temp_s);
  }
  ds1.requestTempAll();
}

void readAllDHT() {
  readDHT(dht1, "dht1");
  readDHT(dht2, "dht2");
  readDHT(dht3, "dht3");
  readDHT(dht4, "dht4");
}

void readDHT(DHT dht, String dht_name) {
  float t = dht.readTemperature();
  String str_t = isnan(t) ? "null" : String(t);  
  mqttSend(dht_name + "_t", str_t);

  float h = dht.readHumidity();
  String str_h = isnan(h) ? "null" : String(h);  
  mqttSend(dht_name + "_h", str_h);
}

void mqttSend (String name, String msg) {
  String topic = "sensors/" + name;
  Serial.println(topic.c_str());
  Serial.println(msg.c_str());
  mqttClient.publish(topic.c_str(), msg.c_str(), true);
}

void blink(byte count = 1) {
  for (byte i=0;  i<count; i++) {
    digitalWrite(LED_BUILTIN, LOW);
    delay(50);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(200);
  }
}
