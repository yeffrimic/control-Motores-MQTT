/*
   Control de motores con l293D y NodeMCU
   Este sketch controla 2 motores
   con el chip l293d conectado
   a un nodemcu por medio de mqtt y node-red
   suscrito al topico
   componentes:
   NodeMCU
   l293D

*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define motor1Adelante D7
#define motor1Atras D6
#define motor2Adelante D5
#define motor2Atras D8

const char* ssid = "ESP8266";
const char* password = "11111111";
const char* mqtt_server = "45.55.83.183";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

void setup() {
  pinMode(motor1Adelante, OUTPUT);
  pinMode(motor2Atras, OUTPUT);
  pinMode(motor1Atras, OUTPUT);
  pinMode(motor2Adelante, OUTPUT);
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void setup_wifi() {

  delay(10);
  Serial.println();
  Serial.print("conectando a  ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("direccion IP: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("mensaje recibido  [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  if ((char)payload[0] == 'a') {
    Serial.println("izquierda");
    digitalWrite(motor1Adelante,LOW);
    digitalWrite(motor2Adelante,HIGH);
    digitalWrite(motor1Atras,LOW);
    digitalWrite(motor2Atras,LOW);
    
  }
  if ((char)payload[0] == 'w') {
    Serial.println("adelante");
    digitalWrite(motor1Adelante,HIGH);
    digitalWrite(motor2Adelante,HIGH);
    digitalWrite(motor1Atras,LOW);
    digitalWrite(motor2Atras,LOW);
  }
  if ((char)payload[0] == 'd') {
    Serial.println("derecha");
    digitalWrite(motor1Adelante,HIGH);
    digitalWrite(motor2Adelante,LOW);
    digitalWrite(motor1Atras,LOW);
    digitalWrite(motor2Atras,LOW);
  }
  if ((char)payload[0] == 's') {
    Serial.println("atras");
    digitalWrite(motor1Adelante,LOW);
    digitalWrite(motor2Adelante,LOW);
    digitalWrite(motor1Atras,HIGH);
    digitalWrite(motor2Atras,HIGH);
  }
  if ((char)payload[0] == 'q') {
    Serial.println("parar");
    digitalWrite(motor1Adelante,LOW);
    digitalWrite(motor2Adelante,LOW);
    digitalWrite(motor1Atras,LOW);
    digitalWrite(motor2Atras,LOW);
  }

}

void reconnect() {
  // ciclo para conectarnos al mqtt
  while (!client.connected()) {
    Serial.print("conectando a mqtt...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("conectado");
      // una vez conectado, publicamos que estamos conectados al topico /conexion
      client.publish("/conexion", "conectado");
      // ... y nos suscribimos a /movimiento
      client.subscribe("/movimiento");
    } else {
      Serial.print("fallo, rc=");
      Serial.print(client.state());
      Serial.println("reintentar en 5 segundos");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

}
