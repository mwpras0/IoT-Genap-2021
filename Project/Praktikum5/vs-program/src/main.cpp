#include <Arduino.h>
#include <DHT.h>
#define DHTTYPE DHT11
DHT dht(D7, DHTTYPE);
#define RED_LED D1 //led warna merah
#define GREEN_LED D2 //led warna hijau
#define BLUE_LED D3 //led warnah biru

void setup() {
    Serial.begin(115200);
    pinMode(RED_LED,OUTPUT);
    pinMode(GREEN_LED,OUTPUT);
    pinMode(BLUE_LED,OUTPUT);
    Serial.println("Menggunakan DHT11 dengan LED");
    dht.begin();
}
void loop() {
    delay(2000);
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    float f = dht.readTemperature(true);
    if (isnan(h) || isnan(t) || isnan(f))
    {
        Serial.println("Failed to read from DHT sensor!");
        digitalWrite(RED_LED, HIGH);
        digitalWrite(GREEN_LED, HIGH);
        digitalWrite(BLUE_LED, HIGH);
        return;
    }

    float hif = dht.computeHeatIndex(f, h);
    float hic = dht.computeHeatIndex(t, h, false);

    Serial.print(F("Humidity: "));
    Serial.print(h);
    Serial.print(F("% Temperature: "));
    Serial.print(t);
    Serial.print(F("°C"));
    Serial.print(t);
    Serial.print(F("°F Heat index: "));
    Serial.print(hic);
    Serial.print(F("°C "));
    Serial.print(hif);
    Serial.println(F("°F"));

    if(t >= 40){
      digitalWrite(RED_LED, LOW);
      digitalWrite(GREEN_LED, HIGH);
      digitalWrite(BLUE_LED, HIGH);
      Serial.println("Merah = Panas");
    }else if(t >= 25){
      digitalWrite(RED_LED, HIGH);
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(BLUE_LED, HIGH);
      Serial.println("Hijau = Normal");
    }else{
      digitalWrite(RED_LED, HIGH);
      digitalWrite(GREEN_LED, HIGH);
      digitalWrite(BLUE_LED, LOW);
      Serial.println("Biru = Dingin");
    }
}