#include <Arduino.h>
#include <DHT.h>

#define sensorLDR A0 //sensor cahaya
#define DHTTYPE DHT11 //sensor kelembapan
#define RED_LED D5 //Led Red
#define BLUE_LED D7 //Led Blue

DHT dht(D2, DHTTYPE);
int nilaiSensor;

void setup() {
  Serial.begin(115200);
  pinMode(RED_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  Serial.println("Penggunaan Sensor LDR dan Sensor DHT11");
  delay(3000);
}

void blue()
{
  for (int x = 0; x < 3; x++)
  {
    digitalWrite(BLUE_LED, LOW); // LED nyala
    delay(150);
    digitalWrite(BLUE_LED, HIGH); // LED mati
    delay(100);
  }
  Serial.print("LED Biru menyala");
  Serial.print("| Suhu Dingin");
  Serial.print("| Cahaya redup");
  Serial.println();
  delay(1000);
}

void dead()
{
  digitalWrite(RED_LED, HIGH);
  digitalWrite(BLUE_LED, HIGH);
  delay(1000);
}

void red()
{
  digitalWrite(RED_LED, LOW);
  Serial.print("LED Merah menyala");
  Serial.print("| Suhu Panas");
  Serial.print("| Cahaya Terang");
  Serial.println();
  delay(1000);
}

void loop() {
  delay(2000);
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t))
  {
    Serial.println("Failed to read from DHT sensor!");
    dead();
    return;
  }

  float hic = dht.computeHeatIndex(t, h, false);
  nilaiSensor = analogRead(sensorLDR);
  Serial.print("Nilai Sensor : ");
  Serial.print(nilaiSensor);
  Serial.print(F(" Humidity: "));
  Serial.print(h);
  Serial.print(F("% Temperature: "));
  Serial.print(hic);
  Serial.println();
  delay(1000);

  if (t > 30 && nilaiSensor < 500)
  {
    red();
    delay(100);
  }

  else if (t < 20 && nilaiSensor > 500)
  {
    blue();
    delay(100);
  }

  else
  {
    dead();
  }
}