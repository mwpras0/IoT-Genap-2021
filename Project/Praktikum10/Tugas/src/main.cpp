#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <ESP8266WiFi.h>
#include <SimpleDHT.h>

#define pinDHT 7
#define sensorLDR A0
#define RED_LED D5
#define GREEN_LED D6
#define BLUE_LED D7

byte temperature = 0;
byte humidity = 0;
SimpleDHT11 dht11(D1); //instan sensor dht11

int nilaiSensor;

const char *ssid = "ccrsxx";
const char *password = "bilangapa";
const uint16_t port = 9000;
const char *host = "ec2-54-234-21-155.compute-1.amazonaws.com";

WiFiClient client;

void connect_wifi()
{
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");
  delay(250);
}

void connect_server()
{
  while (!client.connect(host, port))
  {
    Serial.printf("\n[Connecting to %s ... ", host);
    delay(1000);
    return;
  }
  Serial.println("connected]");
  delay(1000);
}

void setup() {
  Serial.begin(115200);
  Serial.println("Penggunaan socket client");
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  connect_wifi();
  connect_server();
}

void LDR(){
  nilaiSensor = analogRead(sensorLDR);
  Serial.print("Nilai Sensor : ");
  Serial.println(nilaiSensor);
  
  if(nilaiSensor < 500){
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, HIGH);
  }else{
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(BLUE_LED, HIGH);
  }
  delay(1000);
}

void cahaya(){
  if(temperature > 30 || nilaiSensor < 500){
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(BLUE_LED, HIGH);
  }else{
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(BLUE_LED, HIGH);
  }
}

void KelembabanSuhu(){
  int err = SimpleDHTErrSuccess;
  
  if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess){
    Serial.print("Pembacaan DHT11 gagal, err=");
    Serial.println(err);
    delay(1000);
    return;
  }

  Serial.print("Sample OK: ");
  Serial.print((int)temperature);

  Serial.print(" *C, ");
  Serial.print((int)humidity);
  Serial.println(" H");

  delay(2000);
}

void loop()
{
  if (client.connected())
  {
    Serial.println("[Response:]");
    KelembabanSuhu();
    cahaya();
    String line = client.readStringUntil('\n');
    Serial.println(line);
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(BLUE_LED, HIGH);
    if(line.equalsIgnoreCase("lampu")){
      digitalWrite(RED_LED, HIGH);
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(BLUE_LED, LOW);
    }else if(line.equalsIgnoreCase("sensor cahaya")){
      LDR();
    }
  }else{
    connect_server();
  }
}