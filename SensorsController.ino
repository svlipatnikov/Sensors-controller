#include <microDS18B20.h>
#define DS_PIN D3 // пин для термометров

// Уникальные адреса датчиков - считать можно в примере address_read
uint8_t s1_addr[] = { 0x28, 0x0F, 0x51, 0x16, 0xA8, 0x01, 0x3C, 0x59 }; //kit
uint8_t s1_addr[] = { 0x28, 0x36, 0x74, 0x16, 0xA8, 0x01, 0x3C, 0x08 }; //bed
uint8_t s1_addr[] = { 0x28, 0x3C, 0xA6, 0x16, 0xA8, 0x01, 0x3C, 0x1A }; //child
uint8_t s1_addr[] = { 0x28, 0x02, 0x74, 0x16, 0xA8, 0x01, 0x3C, 0x39 }; //bath
MicroDS18B20<DS_PIN, s1_addr> sensor1;  // Создаем термометр с адресацией

void setup() {
  Serial.begin(115200);
}

void loop() {
  // асинхронное чтение нескольких датчиков смотри в примере async_read_many
  sensor1.requestTemp();      // Запрашиваем преобразование температуры

  delay(1000);           // ожидаем результат 
  
  Serial.print("t1: ");
  if (sensor1.readTemp()) Serial.println(sensor1.getTemp());
  else Serial.println("error");

}