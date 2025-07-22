#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// Configuración WiF
const char* ssid = "";     
const char* password = "";      
//const uint8_t channel = 11;              
const int wifi_timeout = 30;         

#define DHTPIN D4                      
#define DHTTYPE DHT22                    
DHT dht(DHTPIN, DHTTYPE);

const char* mqtt_server = ""; 
const int mqtt_port = 1883;
const char* mqtt_topic = "sensor/dht";

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastReconnectAttempt = 0;
const unsigned long reconnectInterval = 5000; // 5 segundos entre reconexiones

void setup() {
  Serial.begin(115200);
  delay(2000); /
  
  initWiFi();
  initDHT();
  initMQTT();
}

void loop() {
  checkWiFiConnection();
  checkMQTTConnection();
  
  static unsigned long lastReading = 0;
  if (millis() - lastReading > 10000) { 
    lastReading = millis();
    readAndPublishData();
  }
  
  client.loop();
  delay(100); 
}


void initWiFi() {
  
  WiFi.setSleepMode(WIFI_NONE_SLEEP);
  WiFi.setPhyMode(WIFI_PHY_MODE_11G); 
  WiFi.setOutputPower(20.5);          
  
  WiFi.begin(ssid, password, channel);
  
  unsigned long startTime = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - startTime < wifi_timeout * 1000) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWiFi conectado");
    Serial.printf("IP: %s\n", WiFi.localIP().toString().c_str());
    Serial.printf("RSSI: %d dBm\n", WiFi.RSSI());
  } else {
    Serial.println("\nFallo de conexión WiFi");
    diagnosticarError();
  }
}

void initDHT() {
  Serial.println("Inicializacion del sensor"");
  dht.begin();
  delay(2000);
  
  float testHum = dht.readHumidity();
  float testTemp = dht.readTemperature();
  
  if (isnan(testHum) || isnan(testTemp)) {
    Serial.println(" Error al inicializar el sensor DHT22");
  } else {
    Serial.println("Sensor DHT22 listo");
  }
}

void initMQTT() {
  Serial.println("\n=== CONFIGURACIÓN MQTT ===");
  client.setServer(mqtt_server, mqtt_port);
  
  if (reconnectMQTT()) {
    Serial.println("Conexión MQTT establecida");
  } else {
    Serial.println( "Fallo al conectar a MQTT");
  }
}

void checkWiFiConnection() {
  if (WiFi.status() != WL_CONNECTED) {
    unsigned long now = millis();
    if (now - lastReconnectAttempt > reconnectInterval) {
      lastReconnectAttempt = now;
      Serial.println("⚠️ Reconectando WiFi...");
      WiFi.reconnect();
      
      if (WiFi.status() == WL_CONNECTED) {
        Serial.println(" WiFi reconectado");
      }
    }
  }
}

bool reconnectMQTT() {
  if (!client.connected()) {
    
    if (client.connect("ESP8266Client")) {
      Serial.println("Conectado");
      return true;
    } else {
      Serial.printf("Fallo (código %d)\n", client.state());
      return false;
    }
  }
  return true;
}

void checkMQTTConnection() {
  if (!client.connected()) {
    unsigned long now = millis();
    if (now - lastReconnectAttempt > reconnectInterval) {
      lastReconnectAttempt = now;
      reconnectMQTT();
    }
  }
}

void readAndPublishData() {
  if (WiFi.status() == WL_CONNECTED && client.connected()) {
    float hum = dht.readHumidity();
    float temp = dht.readTemperature();
    
    if (!isnan(hum) && !isnan(temp)) {
      String payload = "{\"temperature\":" + String(temp, 1) + ",\"humidity\":" + String(hum, 1) + "}";
      Serial.println(" Enviando: " + payload);
      
      if (client.publish(mqtt_topic, payload.c_str())) {
        Serial.println("Datos publicados");
      } else {
        Serial.println(" Fallo al publicar");
      }
    } else {
      Serial.println("Lectura inválida del sensor");
    }
  }
}

void diagnosticarError() {
  Serial.println("\n Diagnositco:");
  
  int n = WiFi.scanNetworks(false, true);
  bool redVisible = false;
  
  for (int i = 0; i < n; ++i) {
    if (WiFi.SSID(i) == ssid) {
      redVisible = true;
      Serial.printf("Red detectada: %s (Canal %d) %d dBm\n", 
                   WiFi.SSID(i).c_str(), 
                   WiFi.channel(i), 
                   WiFi.RSSI(i));
    }
  }
  

  
  Serial.println("\n Reiniciando en 5 segundos...");
  delay(5000);
  ESP.restart();
}
