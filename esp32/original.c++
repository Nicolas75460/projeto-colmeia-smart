#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "DHT.h"

// Configurações da rede WiFi
const char* ssid = "WIFI_ADM_CFP402";
const char* password = "ac4ce0ss2";

// Configurações do MQTT
const char* mqtt_server = "10.109.3.164";
const char* mqtt_topic = "device/6f393ee7-7eaf-4453-aee7-e74b96b66167";


/*// Configurações da rede WiFi
const char* ssid = "Sabrina Q";
const char* password = "Casaitape";

// Configurações do MQTT
const char* mqtt_server = "mundobee.ddns.net";
const char* mqtt_topic = "device/6be3392c-3344-44de-b70d-6e99dea554e5";*/

WiFiClient espClient;
PubSubClient client(espClient);

// Configurações do temporizador
const long interval = 1000 * 60 * 10; // 10 minutos em milissegundos (configurável)
unsigned long previousMillis = 0;

// Variáveis para armazenar as contagens acumuladas
int totalInCount = 0;
int totalOutCount = 0;

// Configurações do LCD I2C
LiquidCrystal_I2C lcd(0x27, 16, 4); // Endereço do LCD I2C, 16 colunas e 4 linhas

// Configurações dos sensores DHT
#define DHTPIN1 23     // Pino do sensor interno (D23)
#define DHTPIN2 19    // Pino do sensor externo (D25)
#define DHTPIN3 18     // Pino do sensor externo (D25)
// Configurações do Peltier
#define VENTOINHA 13    // Pino da ventoinha 
#define IN1 12         // Pino do Peltier ESQUENTA
#define IN2 14         // Pino do Peltier ESFRIA
// Configurações dos sensores IR
#define IRSensor1 5   // Pino do primeiro sensor IR (KY-032) saida
#define IRSensor2 2   // Pino do segundo sensor IR (KY-032) entrada


// Variáveis para rastrear o estado dos sensores IR
int irSensor1Count = 0;
int irSensor2Count = 0;

#define DHTTYPE DHT11  // Tipo do sensor DHT11

DHT dht1(DHTPIN1, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);
DHT dht3(DHTPIN3, DHTTYPE);

// Variável para segurança
int seguranca = 0;

// Função de reconexão WiFi
void setup_wifi() {
    delay(10);
    Serial.println();
    Serial.print("Conectando a Wifi");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Conectando a rede ");
    lcd.println(ssid);
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    unsigned long startAttemptTime = millis();

    // Continua tentando conectar por 10 segundos
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
                // Se não conectar em 30 segundos, sai da tentativa de reconexão
                if (millis() - previousMillis > 30000) {
                    break;
                }
            }
        }
    }
}

void setup() {
    Serial.begin(9600);
    setup_wifi();
    client.setServer(mqtt_server, 1883);

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
    }else if(currentIRSensor1State && irSensor2Count == 0){
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
    }else if(currentIRSensor2State && irSensor1Count == 0){
      irSensor2Count++;
    }

    // Lê os dados dos sensores DHT
    float temperature = dht1.readTemperature();
    float humidity = dht1.readHumidity();
    float outsideTemp = dht2.readTemperature();
    float outsideHumidity = dht2.readHumidity();
    float externalTemp = dht3.readTemperature(); // Sensor realmente externo
    float externalHumidity = dht3.readHumidity();

    // Exibe os dados no LCD
    // lcd.createChar(0, abelhaParte1);
    // lcd.createChar(1, abelhaParte2);
    // lcd.createChar(2, abelhaParte3);

    lcd.setCursor(17, 0); // Define o cursor na coluna 0, linha 0
    lcd.write(byte(0));  // Escreve a primeira parte da abelha

    lcd.setCursor(18, 0); // Define o cursor na coluna 1, linha 0
    lcd.write(byte(1));  // Escreve a segunda parte da abelha

    lcd.setCursor(19, 0); // Define o cursor na coluna 1, linha 0
    lcd.write(byte(2));  // Escreve a segunda parte da abelha


    delay(3000);

    lcd.setCursor(0, 0);
    lcd.print("TIN:");
    lcd.print(temperature);
    lcd.print("C");
    lcd.setCursor(0, 1);
    lcd.print("UIN:");
    lcd.print(humidity);
    lcd.print("%");
    lcd.setCursor(0, 2);
    lcd.print("TEN:");
    lcd.print(outsideTemp);
    lcd.print("C");
    lcd.setCursor(0, 3);
    lcd.print("UEN:");
    lcd.print(outsideHumidity);
    lcd.print("%");
    lcd.setCursor(10, 0);
    lcd.print("TEX:");
    lcd.print(externalTemp);
    lcd.print("C");
    lcd.setCursor(10, 1);
    lcd.print("UEX:");
    lcd.print(externalHumidity);

    lcd.setCursor(10, 2);
    lcd.print("IN:");
    lcd.setCursor(10, 3);
    lcd.print("OUT:");

    // Verifica a temperatura para controle de segurança
    if (temperature >= 40) { // Lógica desligar tudo caso tenha um problema
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
        digitalWrite(VENTOINHA, LOW);
        seguranca = 1;
    }else if(temperature >=34 && temperature <= 36 && seguranca == 0){//Parar o Peltier
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
        digitalWrite(VENTOINHA, LOW);  
    } else if (temperature <= 34 && seguranca == 0) {//Aquecimento
        digitalWrite(IN1, HIGH); 
        digitalWrite(IN2, LOW);
        digitalWrite(VENTOINHA, HIGH);
    } else if (temperature >= 36 && seguranca == 0){//Esfriamento
        digitalWrite(IN1, LOW); 
        digitalWrite(IN2, HIGH);
        digitalWrite(VENTOINHA, HIGH);
    }

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
        client.publish(mqtt_topic, jsonBuffer);
    }

    delay(100); // Pequeno atraso para evitar loop muito rápido
}