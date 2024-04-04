#define PIN D0 // - не работает
// #define PIN D1
// #define PIN D2
// #define PIN D3
// #define PIN D4 //  build-in LED
// #define PIN D5
// #define PIN D6
// #define PIN D7
// #define PIN D8 // - не загружается, но работает если подключить после загрузки (boot fails if pulled HIGH)
// #define PIN 1 // TX - Serial log not work


#include "DHT.h"

// Одна из следующих строк закоментирована. Снимите комментарий, если подключаете датчик DHT11 к arduino
DHT dht(PIN, DHT22); //Инициация датчика
//DHT dht(PIN, DHT11);
void setup() {
  Serial.begin(115200);
  dht.begin();
}
void loop() {
  delay(2000); // 2 секунды задержки
  float h = dht.readHumidity(); //Измеряем влажность
  float t = dht.readTemperature(); //Измеряем температуру
  if (isnan(h) || isnan(t)) {  // Проверка. Если не удается считать показания, выводится «Ошибка считывания», и программа завершает работу
    Serial.println("Ошибка считывания");
    return;
  }
  Serial.print("Влажность: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Температура: ");
  Serial.print(t);
  Serial.println(" *C "); //Вывод показателей на экран
}