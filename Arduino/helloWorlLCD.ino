#include <LiquidCrystal.h>

const int RS = 7;
const int EN = 6;
const int D4 = 5;
const int D5 = 4;
const int D6 = 3;
const int D7 = 2;

LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

void setup() {
  lcd.begin(16, 2);
  delay(100);
  lcd.print("Hello World!");
}

void loop() {
  for (int i = 0; i < 24; i++) {
    lcd.scrollDisplayLeft(); 
    delay(500);
  }
  delay(2000);
}
