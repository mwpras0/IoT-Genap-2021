#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
   Serial.begin(115200);
   lcd.init(); // initialize the lcd
   lcd.backlight();
   lcd.home();
}

 void scrollText(int row, String message, int delayTime, int lcdColumns)
 {
   for (int i = 0; i < lcdColumns; i++)
   {
     message = " " + message;
   }
   message = message + " ";
   for (int pos = 0; pos < message.length(); pos++)
   {
     lcd.setCursor(0, row);
     lcd.print(message.substring(pos, pos + lcdColumns));
     delay(delayTime);
   }
 }

void loop() {
   lcd.home();
   lcd.print("Hai Sayang...");
   //Serial.println("Wahyu");
   //delay(250);
   scrollText(1, "Emilia Selvy Saputri", 250, 16);
}