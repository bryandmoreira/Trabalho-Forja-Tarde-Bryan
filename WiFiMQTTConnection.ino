#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiManager.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>


// MQTT
const char *BROKER = "broker.hivemq.com";
const int PORT = 1883;
const char *TOPIC = "forja/desenvolvimento/tarde";

// LED RGB
int RED_PIN = 5;
int GREEN_PIN = 18;
int BLUE_PIN = 19;

WiFiManager wm;
WiFiClient espClient;
PubSubClient client(espClient);
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  digitalWrite(RED_PIN, HIGH);
  digitalWrite(GREEN_PIN, HIGH);
  digitalWrite(BLUE_PIN, HIGH);
  
  lcd.init();
  lcd.backlight();

  Wire.begin(21, 22);

  Serial.println("Conectando WiFi");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Conectando WiFi");
  bool connected = wm.autoConnect("Marco-Setup");
  if (!connected) {
    Serial.println("Falha ao conectar WiFi");
    ESP.restart();
  }
  Serial.println("WiFi conectado!");
  Serial.println(WiFi.localIP());
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi conectado!");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());

  client.setServer(BROKER, PORT);
  client.setCallback(callback);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (!client.connected()) {
    while (!client.connected()) {
      digitalWrite(LED_BUILTIN, HIGH);
      Serial.println("Conectando MQTT...");
      
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Conectando MQTT");
      String clientId = "ESP32-" + String(random(0xffff), HEX);
      if (client.connect(clientId.c_str())) {
        client.subscribe(TOPIC);
        Serial.println("Conectado ao MQTT");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Conectado!");
      } else {
        Serial.print("Conexão falhou: ");
        Serial.println(client.state());
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Conexão falhou:");
        lcd.setCursor(0, 1);
        lcd.print(client.state());
        delay(200);
      }
    }
    digitalWrite(LED_BUILTIN, HIGH);
  }
  client.loop();
}

void callback(char *topic, byte *payload, unsigned int length) {
  Serial.println("Mensagem recebida:");
  String leitura = "";
  for (int i = 0; i < length; i++) {
    leitura += (char)payload[i];
  }
  Serial.println(leitura);

  // Comece seu código aqui
  // {"command": "escrever", "value": "Teste"}
  String command = leitura.substring(0, leitura.indexOf(","));
  String value = leitura.substring(leitura.indexOf(",") + 1);

  command = command.substring(command.indexOf(":") + 1);
  value = value.substring(value.indexOf(":") + 1);  

  command = command.substring(command.indexOf("\"") + 1, commandf.lastIndexOf("\""));
  value = value.substring(value.indexOf("\"") + 1, value.lastIndexOf("\"")); 

  Serial.println("Mensagem filtrada: ");
  Serial.println(command);
  Serial.f(value);
  
  if (command == "Apagar") {
    digitalWrite(RED_PIN, HIGH);
    digitalWrite(RED_PIN, HIGH);
    digitalWrite(RED_PIN, HIGH);
  }
  
  if (command == "Escrever") {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(value);
  } 
  
  if (command == "Reset") {
    wm.resetSettings();
    ESP.restart();
  }

  if (command == "Acender") {
    if (value == "Vermelho") {
        digitalWrite(RED_PIN, LOW);
        digitalWrite(GREEN_PIN, HIGH);
        digitalWrite(BLUE_PIN, HIGH);
    }
    if (value == "Verde") {
        digitalWrite(RED_PIN, HIGH);
        digitalWrite(GREEN_PIN, LOW);
        digitalWrite(BLUE_PIN, HIGH);
    }
    if (value == "Azul") {
        digitalWrite(RED_PIN, HIGH);
        digitalWrite(GREEN_PIN, HIGH);
        digitalWrite(BLUE_PIN, LOW);
    }
    if (value == "Amarelo") {
        digitalWrite(RED_PIN, LOW);
        digitalWrite(GREEN_PIN, LOW);
        digitalWrite(BLUE_PIN, HIGH);
    }
    if (value == "Roxo") {
        digitalWrite(RED_PIN, LOW);
        digitalWrite(GREEN_PIN, HIGH);
        digitalWrite(BLUE_PIN, LOW);
    }
    if (value == "Ciano") {
        digitalWrite(RED_PIN, HIGH);
        digitalWrite(GREEN_PIN, LOW);
        digitalWrite(BLUE_PIN, LOW);
    }

    if (value == "Branco") {
        digitalWrite(RED_PIN, LOW);
        digitalWrite(GREEN_PIN, LOW);
        digitalWrite(BLUE_PIN, LOW);
    }
  }
}
