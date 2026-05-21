#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// Configurações da rede WiFi
const char* ssid = "WIFI_ADM_CFP402";
const char* password = "ac4ce0ss2";

// Configurações do MQTT
const char* mqtt_server = "10.109.3.167";
const char* mqtt_topic = "device/7c05b858-3f77-4c52-a226-7887b7339e16";

char* peltier_status = " ";

WiFiClient espClient;
PubSubClient client(espClient);

// Configurações do temporizador
const long interval = 1000 * 60 * 10; // 10 minutos em milissegundos

unsigned long previousMillis = 0;

// Variáveis para armazenar as contagens acumuladas
int totalInCount = 0;
int totalOutCount = 0;

// Configurações do LCD I2C
LiquidCrystal_I2C lcd(0x27, 16, 4); // Endereço do LCD I2C, 16 colunas e 4 linhas

// Configurações dos sensores DHT
#define DHTPIN1 23     // Pino do sensor interno (D23)
#define DHTPIN2 19     // Pino do sensor externo (D19)
#define DHTPIN3 18     // Pino do sensor externo (D18)
#define DHTTYPE DHT11  // Tipo do sensor DHT11
DHT dht1(DHTPIN1, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);
DHT dht3(DHTPIN3, DHTTYPE);

// Configurações do Peltier
#define VENTOINHA 13    // Pino da ventoinha
#define IN1 12         // Pino do Peltier ESQUENTA
#define IN2 14         // Pino do Peltier ESFRIA

// Configurações dos sensores IR
#define IRSensor1 5   // Pino do primeiro sensor IR (saída)
#define IRSensor2 2   // Pino do segundo sensor IR (entrada)

// Variáveis para rastrear o estado dos sensores IR
int irSensor1Count = 0;
int irSensor2Count = 0;

// Variável para segurança
int seguranca = 0;

// Função de reconexão WiFi
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Conectando a rede ");
  lcd.println(ssid);

  WiFi.begin(ssid, password);

  unsigned long startAttemptTime = millis();

  // Tenta conectar por 10 segundos
  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Conectado ao wifi");
    Serial.println("WiFi conectado");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
    delay(2000);
  } else {
    Serial.println("");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Falha na conexão");
    Serial.println("Falha na conexão WiFi");
    delay(2000);
  }
}

// Função de reconexão MQTT
void reconnect() {
  if (WiFi.status() == WL_CONNECTED) {
    while (!client.connected()) {
      Serial.print("Tentando conexão MQTT...");
      if (client.connect("ESP32Client32")) {
        Serial.println("conectado");
      } else {
        Serial.print("falhou, rc=");
        Serial.print(client.state());
        Serial.println(" tentará novamente em 5 segundos");
        delay(5000);
        // Sai da tentativa após 30 segundos
        if (millis() - previousMillis > 30000) {
          break;
        }
      }
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setKeepAlive(60);

  // Configura os pinos dos sensores IR como entrada
  pinMode(IRSensor1, INPUT);
  pinMode(IRSensor2, INPUT);

  // Inicializa o LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();

  // Inicializa os sensores DHT
  dht1.begin();
  dht2.begin();
  dht3.begin();

  // Configura os pinos do Peltier e ventoinha
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(VENTOINHA, OUTPUT);
}

void loop() {
  if (WiFi.status() == WL_CONNECTED && !client.connected()) {
    reconnect();
  }
  if (client.connected()) {
    client.loop();
  }

  // Obtém o tempo atual
  unsigned long currentMillis = millis();

  // Verifica os estados dos sensores IR
  bool currentIRSensor1State = digitalRead(IRSensor1) == LOW;
  bool currentIRSensor2State = digitalRead(IRSensor2) == LOW;

  // Detecta entradas e saídas com base na sequência de ativação dos sensores IR
  if (currentIRSensor1State && irSensor2Count < 0) {
    irSensor2Count--;
    Serial.println("Sensor IR 1 ativado");
    Serial.println("Saiu uma");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Saiu uma");
    totalOutCount++;
  } else if (currentIRSensor1State && irSensor2Count == 0) {
    irSensor1Count++;
  }

  if (currentIRSensor2State && irSensor1Count < 0) {
    irSensor1Count--;
    Serial.println("Sensor IR 2 ativado");
    Serial.println("Entrou uma");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Entrou uma");
    totalInCount++;
  } else if (currentIRSensor2State && irSensor1Count == 0) {
    irSensor2Count++;
  }

  // Lê os dados dos sensores DHT
  float temperature = dht1.readTemperature();
  float humidity = dht1.readHumidity();
  float outsideTemp = dht2.readTemperature();
  float outsideHumidity = dht2.readHumidity();
  float externalTemp = dht3.readTemperature();
  float externalHumidity = dht3.readHumidity();

  delay(3000);

  // Verifica a temperatura para controle de segurança
    // Verifica a temperatura para controle de segurança
    if (temperature >= 40) { // Lógica desligar tudo caso tenha um problema
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
        digitalWrite(VENTOINHA, LOW);
        seguranca = 1;
        peltier_status = "off(seg.)   ";
    }else if(temperature >=34 && temperature <= 36 && seguranca == 0){//Parar o Peltier
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
        digitalWrite(VENTOINHA, LOW);
        peltier_status = "off         ";
    } else if (temperature <= 34 && seguranca == 0) {//Aquecimento
        digitalWrite(IN1, HIGH); 
        digitalWrite(IN2, LOW);
        digitalWrite(VENTOINHA, HIGH);
        peltier_status = "on(aquec)   ";
    } else if (temperature >= 36 && seguranca == 0){//Esfriamento
        digitalWrite(IN1, LOW); 
        digitalWrite(IN2, HIGH);
        digitalWrite(VENTOINHA, HIGH);
        peltier_status = "on(esfri)   ";
    }

  lcd.setCursor(0, 0);
  lcd.print("TIN:");
  lcd.print(temperature,1);
  lcd.print("C ");

  lcd.setCursor(0, 1);
  lcd.print("TEN:");
  lcd.print(outsideTemp,1);
  lcd.print("C ");

  lcd.setCursor(0, 2);
  lcd.print("TEX:");
  lcd.print(externalTemp,1);
  lcd.print("C ");

  lcd.setCursor(10, 0);
  lcd.print("UIN:");
  lcd.print(humidity,1);
  lcd.print("% ");

  lcd.setCursor(10, 1);
  lcd.print("UEN:");
  lcd.print(outsideHumidity,1);
  lcd.print("% ");

  lcd.setCursor(10, 2);
  lcd.print("UEX:");
  lcd.print(externalHumidity,1);
  lcd.print("% ");

  lcd.setCursor(0, 3);
  lcd.print("PELTIER:");
  lcd.print(peltier_status);

  // Envia dados MQTT a cada intervalo definido
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
  
    StaticJsonDocument<200> doc;
    doc["temperature"] = temperature;
    doc["humidity"] = humidity;
    doc["outsideTemp"] = outsideTemp;
    doc["outsideHumidity"] = outsideHumidity;
    doc["externalTemp"] = externalTemp;
    doc["externalHumidity"] = externalHumidity;
    doc["inCount"] = totalInCount;
    doc["outCount"] = totalOutCount;
  
    char jsonBuffer[512];
    serializeJson(doc, jsonBuffer);
    Serial.print("Publicando no tópico: ");
    Serial.println(mqtt_topic);
    if (client.publish(mqtt_topic, jsonBuffer)) {
      Serial.println("Dados enviados: " + String(jsonBuffer));
    } else {
      Serial.println("Falha ao enviar dados");
    }
  }

  delay(100); // Pequeno atraso para evitar loop muito rápido
}