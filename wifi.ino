const char ssid[] = "welcome's wi-fi";
const char pass[] = "27101988";

const int RESTART_PERIOD = 60 * 60 * 1000;    // время до ребута, если не удается подключиться к wi-fi
const int CHECK_PERIOD = 60 *  1000;          // периодичность проверки на подключение к сервисам
unsigned long lastOnlineTime = 0;             // время когда модуль был онлайн
unsigned long lastCheckTime = 0;              // время крайней проверки подключения к сервисам

//===================================================================================================
// Подключение к wi-fi
  
void ConnectWiFi() {
  WiFi.mode(WIFI_STA);   
  WiFi.begin(ssid, pass); 

  while (WiFi.status() != WL_CONNECTED) { 
    delay(500); 
  }

  Serial.println();
  Serial.println("Connected to Wi-Fi");
  blink(3);
  delay(1000);

  // web-сервер для обновления по воздуху
  httpUpdater.setup(&httpServer);
  httpServer.begin();
} 

//===================================================================================================

void CheckWiFiConnection() { 
  if ((long)millis() - lastCheckTime > CHECK_PERIOD) {
    lastCheckTime = (long)millis(); 

    if (WiFi.status() == WL_CONNECTED) { 
      lastOnlineTime = (long)millis();  
    } else {
      WiFi.disconnect();
      WiFi.begin(ssid, pass);

      if ((long)millis() - lastOnlineTime > RESTART_PERIOD) {  
        ESP.restart();
      }
    } 
  }  
}

