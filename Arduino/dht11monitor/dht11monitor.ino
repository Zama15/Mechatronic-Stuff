#include <LiquidCrystal.h>
#include <DHT.h>

// DHT PINS
#define DHTPIN 8
#define DHTTYPE DHT11

// LCD PINS
const int RS = 7;
const int EN = 6;
const int D4 = 5;
const int D5 = 4;
const int D6 = 3;
const int D7 = 2;

// OBJECTS
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);
DHT dht(DHTPIN, DHTTYPE);

// LED PINS
const int led = 12;

// BUTTONS PINS
const int move = 11;

const int addMax = 10;
const int addMin = 9;

// VARIABLES
int menu = 1;

char brightness = 0;
unsigned long previousMillisLet = 0;
unsigned long btnDebounceMajor = 2000;
unsigned long btnDebounce = 200;
bool buttonsBlocked = false;

// TEMPERATURE VARIABLES
int max = 40;
int maxMax = 50;
int min = 20;
int minMax = 30;

// int temperature = 0;
float temperature = 0;

// HUMIDITY VARIABLES
int maxHz = 70;
int maxHzMax = 80;
int minHz = 30;
int minHzMax = 40;

// int humidity = 0;
float humidity = 0;

// CUSTOM CHARACTERS
byte degreeSymbol[8] = {
  B00110,
  B01001,
  B01001,
  B00110,
  B00000,
  B00000,
  B00000,
  B00000
};

void setup() {
  // BUTTONS
  pinMode(move, OUTPUT);
  pinMode(addMax, OUTPUT);
  pinMode(addMin, OUTPUT);
  // LED
  pinMode(led, OUTPUT);
  // LCD
  lcd.begin(16, 2);
  delay(50);
  loadCustomCharacter(0, degreeSymbol);
  displayMenu();
  // DHT
  dht.begin();
  delay(50);
  // EXTRA
  Serial.begin(9600);
}

void loop() {
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();

  if (!isnan(humidity) || !isnan(temperature)) {
    Serial.println(String(temperature) + "/" + String(humidity));
  }

  if (Serial.available() > 0) {
    brightness = Serial.read();
  }

  switch (brightness)
  {
  case '0':
    analogWrite(led, 0);
    break;
  case '1':
    analogWrite(led, 20);
    break;
  case '2':
    analogWrite(led, 40);
    break;
  case '3':
    analogWrite(led, 60);
    break;
  case '4':
    analogWrite(led, 80);
    break;
  case '5':
    analogWrite(led, 100);
    break;
  case '6':
    analogWrite(led, 120);
    break;
  case '7':
    analogWrite(led, 140);
    break;
  case '8':
    analogWrite(led, 160);
    break;
  case '9':
    analogWrite(led, 180);
    break;
  default:
    break;
  }

  delay(1000);
  
  displayMenu();
}

bool btnPushed() {
  if (digitalRead(move) == HIGH) {
    // Serial.println("Button \"move\" pushed " + String(move));
    moveMenu();
    displayMenu();
    delay(btnDebounceMajor);
    return true;
  } else if (digitalRead(addMax) == HIGH) {
    if (menu == 2) {
      // Serial.println("Button \"addMax Temperature\" pushed " + String(addMax));
      addMaxValue();
      displayMenu();
      delay(btnDebounce);
      return false;
    } else if (menu == 3) {
      // Serial.println("Button \"addMax Humidity\" pushed " + String(addMax));
      addMaxHzValue();
      displayMenu();
      delay(btnDebounce);
      return false;
    }
  } else if (digitalRead(addMin) == HIGH) {
    if (menu == 2) {
      // Serial.println("Button \"addMin Temperature\" pushed " + String(addMin));
      addMinValue();
      displayMenu();
      delay(btnDebounce);
      return false;
    } else if (menu == 3) {
      // Serial.println("Button \"addMin Humidity\" pushed " + String(addMin));
      addMinHzValue();
      displayMenu();
      delay(btnDebounce);
      return false;
    }
  }

  return false;
}

void moveMenu() {
  menu++;
  if (menu > 3) {
    menu = 1;
  }
}

void addMaxValue() {
  max++;
  if (max > maxMax) {
    max = 30;
  }
}

void addMaxHzValue() {
  maxHz++;
  if (maxHz > maxHzMax) {
    maxHz = 80;
  }
}

void addMinValue() {
  min++;
  if (min > minMax) {
    min = 10;
  }
}

void addMinHzValue() {
  minHz++;
  if (minHz > minHzMax) {
    minHz = 20;
  }
}

void loadCustomCharacter(uint8_t charIndex, const uint8_t charData[]) {
  uint8_t data;
  lcd.createChar(charIndex, charData);
}

void printLCD(String text,int col = 0, int row = 0) {
  lcd.setCursor(col, row);
  lcd.print(text);
}

void writeCustomCharacter(int charIndex, int col, int row) {
  lcd.setCursor(col, row);
  lcd.write(byte(charIndex));
}

void displayMenu() {
  lcd.clear();
  switch (menu) {
    case 1:
      writeCustomCharacter(0, 0, 0);
      printLCD("C: " + String(temperature), 1, 0);
      lcd.write(byte(0));
      lcd.print(" " + String(brightness));
      printLCD("Hz: " + String(humidity) + "%", 0, 1);
      break;
    case 2:
      printLCD("Temperature", 0, 0);
      printLCD("Min:" + String(min), 0, 1);
      lcd.write(byte(0));
      lcd.print(" Max:" + String(max));
      lcd.write(byte(0));
      break;
    case 3:
      printLCD("Humidity", 0, 0);
      printLCD("Min:" + String(minHz) + "% Max:" + String(maxHz) + "%", 0, 1);
      break;
  }
}
