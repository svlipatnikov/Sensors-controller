// #define PIN D0
// #define PIN D1
// #define PIN D2
// #define PIN D3
// #define PIN D4 //  build-in LED
// #define PIN D5
// #define PIN D6
// #define PIN D7
// #define PIN D8 // - boot fails if pulled HIGH 
#define PIN 1 // TX - Serial log not work

#include <microDS18B20.h>

MicroDS18B20 <PIN> sensor;  // Создаем термометр без адреса на пине D2
uint8_t address[8];        // Создаем массив для адреса

void setup() { 
  // Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // читаем адрес термометра в указанный массив
  if (sensor.readAddress(address)) {  // если успешно, выводим
    // Serial.print('{');
    // for (uint8_t i = 0; i < 8; i++) {
    //   Serial.print("0x");
    //   Serial.print(address[i], HEX);  // Выводим адрес
    //   if (i < 7) Serial.print(", ");
    // }
    // Serial.println('}');

    digitalWrite(LED_BUILTIN, HIGH);

  } else {
    // Serial.println("Not connected");
  }
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000);
}