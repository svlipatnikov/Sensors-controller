
const char *mqtt_server = "192.168.1.143";         // Адрес сервера MQTT
const int   mqtt_port = 912;                       // Порт для подключения к серверу MQTT
const char *mqtt_user = "sv.lipatnikov";           // Логин от сервера
const char *mqtt_pass = "123456";                  // Пароль от сервера
const char* mqtt_client_name = "Sensors[NodeMCU]"; // Имя клиента

const int MQTT_RECONNECT_TIMEOUT = 5000;           // Пауза перед повторным подключением 

// =========================================================================================
// Подключение к MQTT

void MqttInit() {
  mqttClient.setServer(mqtt_server, mqtt_port);
  mqttClient.setCallback(MqttGet);
  delay(1000);
}

void MqttHandler() {
  if (mqttClient.connected()) {
    mqttClient.loop();
  } else {
    ConnectMQTT();
  }
}

void ConnectMQTT() {
  if (WiFi.status() == WL_CONNECTED) {  
    if (mqttClient.connect(mqtt_client_name, mqtt_user, mqtt_pass)) {
      Serial.println("Connected to mqtt");
      blink(5);
      delay(1000);
    } else {
      delay(MQTT_RECONNECT_TIMEOUT);
    }
  }
}

void MqttGet(char* topic, byte* payload, unsigned int length) {}


