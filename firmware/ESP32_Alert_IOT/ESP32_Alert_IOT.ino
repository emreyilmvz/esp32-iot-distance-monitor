#include <WiFi.h>
#include <PubSubClient.h>
#include "secrets.h"

// Define these values in secrets.h. Never commit that file.
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;
const char* mqtt_server = MQTT_SERVER;
const char* mqtt_username = MQTT_USERNAME;
const char* mqtt_password = MQTT_PASSWORD;

const int trigPin = 25; //trig d25de
const int echoPin = 26; //echo d26da

WiFiClient espClient; //mqtt için tünel
PubSubClient client(espClient);

long sure;
int mesafe;
int sonMesafe = 0;

void setup() {
  Serial.begin(115200);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(2, OUTPUT); // espnin ledi

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Wifi bağlandı");

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback); //mesaj gelince çalışacak
}

//mesaj gelince çalışacak
void callback(char* topic, byte* payload, unsigned int length) {
  String komut = "";
  for (int i = 0; i < length; i++) komut += (char)payload[i];

  // 1 gelirse yak
  if (komut == "1") {
    digitalWrite(2, LOW);
    Serial.println("led söndü");
  }
  // 0 gelirse söndür
  else if (komut == "0") {
    digitalWrite(2, HIGH);
    Serial.println("led yandı");
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("mqtt bağlanıyor");
    if (client.connect("ESP32_Mesafe", mqtt_username, mqtt_password)) {
      Serial.println("mqtt bağlandı");
      client.subscribe("ev/led"); //lede abone ol
    } else {
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();

  //mesafe ölçümü
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  sure = pulseIn(echoPin, HIGH);
  mesafe = sure * 0.0343 / 2;

  //filtre
  if (abs(mesafe - sonMesafe) > 2) { //değişiklik payı 2cm
    String mesafeStr = String(mesafe);
    char msg[10];
    mesafeStr.toCharArray(msg, 10);

    client.publish("ev/mesafe", msg);

    Serial.println("Mesafe: " + String(msg));

    sonMesafe = mesafe;
  }
  delay(100);
}
