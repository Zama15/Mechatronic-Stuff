#include <LiquidCrystal.h>

const int RS = 7;
const int EN = 6;
const int D4 = 5;
const int D5 = 4;
const int D6 = 3;
const int D7 = 2;

LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

const int letInitialize = 22;
const int letCalibrate = 23;
const int letMeasure = 24;

const int select = 28;
const int move = 29;

int menu = 1;
String text = "Initialize";
unsigned long previousMillisLet = 0;
unsigned long letOnTime = 5000;
unsigned long btnDebounce = 200;
bool buttonsBlocked = false;

void setup() {
  // LETS
  pinMode(letInitialize, INPUT);
  pinMode(letCalibrate, INPUT);
  pinMode(letMeasure, INPUT);
  // BUTTONS
  pinMode(select, OUTPUT);
  pinMode(move, OUTPUT);
  // LCD
  lcd.begin(16, 2);
  delay(100);
  lcd.print(text);
  // EXTRA
  Serial.begin(9600);
}

void loop() {
  if (!buttonsBlocked) {
    if (btnPushed() ) {
      buttonsBlocked = true;
      selectMenu();
    }
  } else {
    if (millis() - previousMillisLet >= letOnTime) {
      buttonsBlocked = false;
      displayMenu();
    }
    turnOffAll();
  }
}

void turnOffAll() {
  digitalWrite(letInitialize, LOW);
  digitalWrite(letCalibrate, LOW);
  digitalWrite(letMeasure, LOW);
}

void turnOn(int led) {
  Serial.println("Turning on LED " + String(led));
  digitalWrite(led, HIGH);
}

bool btnPushed() {
  if (digitalRead(select) == HIGH) {
    Serial.println("Button \"select\" pushed " + String(select));
    delay(100);
    return true;
  } else if (digitalRead(move) == HIGH) {
    Serial.println("Button \"move\" pushed " + String(move));
    moveMenu();
    displayMenu();
    delay(1000);
    return false;
  }
  return false;
}

void moveMenu() {
  menu++;
  if (menu > 3) {
    menu = 1;
  }
}

void displayMenu() {
  switch (menu) {
    case 1:
      text = "Initialize";
      break;
    case 2:
      text = "Calibrate";
      break;
    case 3:
      text = "Measure";
      break;
  }
  lcd.clear();
  lcd.print(text);
}

void selectMenu() {
  switch (menu) {
    case 1:
      turnOn(letInitialize);
      dotsAnimation("INITIALIZING");
      break;
    case 2:
      turnOn(letCalibrate);
      dotsAnimation("CALIBRATING");
      break;
    case 3:
      turnOn(letMeasure);
      dotsAnimation("MEASURING");
      break;
  }
}

void dotsAnimation(String text) {
  int cursor = text.length();
  for (int i = 0; i < 3; i++) {
    lcd.clear();
    lcd.print(text);
    lcd.setCursor(cursor, 0);
    lcd.print(".");
    delay(500);
    lcd.setCursor(cursor, 0);
    lcd.print("..");
    delay(500);
    lcd.setCursor(cursor, 0);
    lcd.print("...");
    delay(500);
  }
}
