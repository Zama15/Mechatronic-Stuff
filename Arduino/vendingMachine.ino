// Segments variables
int segmentA = 1;
int segmentB = 2;
int segmentC = 3;
int segmentD = 4;
int segmentE = 5;
int segmentF = 6;
int segmentG = 7;

// Digits variables
int digit1 = 8;
int digit2 = 9;
int digit3 = 10;
int digit4 = 11;

// Buttons dollar variables
int button1dollar = 22;
int button2dollar = 23;
int button5dollar = 24;
int button10dollar = 25;

// Buttons soda variables
int buttonAsoda = 30;
int buttonBsoda = 31;
int buttonCsoda = 32;

// Leds soda variables
int buttonAsodaLed = 40;
int buttonBsodaLed = 41;
int buttonCsodaLed = 42;

// Helper variables
int patternDisplayDigit1 = 0;
int patternDisplayDigit2 = 0;
int patternDisplayDigit3 = 0;
int patternDisplayDigit4 = 0;
int money = 0;
int remainingAmount = 0;
bool waitingForResetAfterSoda = false;
String selectedDrink = "";

// Time variables
unsigned long previousMillis = 0; // Store the last time the display was updated
unsigned long displayDuration = 1000; // Time to display any message on the display
unsigned long buttonDollarsDebounce = 200; // Debounce time for the dollar buttons
unsigned long lastButtonPressTime = 0; // Variable to store the last time a button was pressed
unsigned long displayDelay = 1; // Variable to store the time the display was updated

// Digits patterns
const int digitPatterns[14] = {
  B0111111, // 0 
  B0000110, // 1 
  B1011011, // 2 
  B1001111, // 3 
  B1100110, // 4 
  B1101101, // 5
  B1111101, // 6
  B0000111, // 7
  B1111111, // 8
  B1101111, // 9
  B1110111, // A
  B0111001, // C
  B1110001, // F
  B1000000 // -
};

void setup() {
  pinMode(segmentA, OUTPUT);
  pinMode(segmentB, OUTPUT);
  pinMode(segmentC, OUTPUT);
  pinMode(segmentD, OUTPUT);
  pinMode(segmentE, OUTPUT);
  pinMode(segmentF, OUTPUT);
  pinMode(segmentG, OUTPUT);

  pinMode(digit1, OUTPUT);
  pinMode(digit2, OUTPUT);
  pinMode(digit3, OUTPUT);
  pinMode(digit4, OUTPUT);
  
  pinMode(button1dollar, OUTPUT); 
  pinMode(button2dollar, OUTPUT); 
  pinMode(button5dollar, OUTPUT); 
  pinMode(button10dollar, OUTPUT); 

  pinMode(buttonAsoda, OUTPUT);
  pinMode(buttonBsoda, OUTPUT);
  pinMode(buttonCsoda, OUTPUT);
  
  pinMode(buttonAsodaLed, OUTPUT);
  pinMode(buttonBsodaLed, OUTPUT);
  pinMode(buttonCsodaLed, OUTPUT);
}

void loop() {
  // Turn off all the LEDs
  digitalWrite(buttonAsodaLed, LOW);
  digitalWrite(buttonBsodaLed, LOW);
  digitalWrite(buttonCsodaLed, LOW);
  
  // Read the buttons
  if (digitalRead(button1dollar) == HIGH) {
    money += 1;
    delay(buttonDollarsDebounce);
  }
  if (digitalRead(button2dollar) == HIGH) {
    money += 2;
    delay(buttonDollarsDebounce);
  }
  if (digitalRead(button5dollar) == HIGH) {
    money += 5;
    delay(buttonDollarsDebounce);
  }
  if (digitalRead(button10dollar) == HIGH) {
    money += 10;
    delay(buttonDollarsDebounce);
  }

  // Check if the total amount is enough to buy a soda
  if (selectedDrink == "A") {
    // Turn on the LEDs for the sodas
    digitalWrite(buttonAsodaLed, HIGH);
    digitalWrite(buttonBsodaLed, LOW);
    digitalWrite(buttonCsodaLed, LOW);
  } else if (selectedDrink == "B") {
    // Turn on the LEDs for the sodas
    digitalWrite(buttonAsodaLed, LOW);
    digitalWrite(buttonBsodaLed, HIGH);
    digitalWrite(buttonCsodaLed, LOW);
  } else if (selectedDrink == "C") {
    // Turn on the LEDs for the sodas
    digitalWrite(buttonAsodaLed, LOW);
    digitalWrite(buttonBsodaLed, LOW);
    digitalWrite(buttonCsodaLed, HIGH);
  } else {
    // Turn off the LEDs for the sodas
    digitalWrite(buttonAsodaLed, LOW);
    digitalWrite(buttonBsodaLed, LOW);
    digitalWrite(buttonCsodaLed, LOW);
  }

  // Display the amount of money
  displaymoney();
}

void displaymoney() {
  // Check if a soda was selected and is being displayed
  if (waitingForResetAfterSoda) {
    // Check if the display duration has passed to reset the display
    if (millis() - lastButtonPressTime >= displayDuration) {
      patternDisplayDigit1 = 0; // -
      patternDisplayDigit2 = 0; // -
      patternDisplayDigit3 = 0; // -
      patternDisplayDigit4 = 0; // -
      waitingForResetAfterSoda = false;
      selectedDrink = "";
    }
  } else {
    // If the total amount is enough to buy a soda, display the letters A, B, and C or the remaining amount
    if (money >= 15) {
      patternDisplayDigit1 = (money / 1000) % 10;
      patternDisplayDigit2 = (money / 100) % 10;
      patternDisplayDigit3 = (money / 10) % 10;
      patternDisplayDigit4 = money % 10;
      
    } else {
      patternDisplayDigit1 = (money / 1000) % 10;
      patternDisplayDigit2 = (money / 100) % 10;
      patternDisplayDigit3 = (money / 10) % 10;
      patternDisplayDigit4 = money % 10;
      
    }

    // Check if a soda was selected
    if (digitalRead(buttonAsoda) == HIGH || digitalRead(buttonBsoda) == HIGH || digitalRead(buttonCsoda) == HIGH) {
      lastButtonPressTime = millis(); 
      waitingForResetAfterSoda = true;
    }

    // Check if a soda was selected and display the selected soda
    if (millis() - lastButtonPressTime < displayDuration && waitingForResetAfterSoda) {
      if (digitalRead(buttonAsoda) == HIGH) {
        patternDisplayDigit1 = 0; // -
        patternDisplayDigit2 = 0; // -
        patternDisplayDigit3 = 0; // -
        patternDisplayDigit4 = 10; // A
        money -= 15;
        selectedDrink = "A";
      } else if (digitalRead(buttonBsoda) == HIGH) {
        patternDisplayDigit1 = 0; // -
        patternDisplayDigit2 = 0; // -
        patternDisplayDigit3 = 0; // -
        patternDisplayDigit4 = 11; // B
        money -= 15;
        selectedDrink = "B";
      } else if (digitalRead(buttonCsoda) == HIGH) {
        patternDisplayDigit1 = 0; // -
        patternDisplayDigit2 = 0; // -
        patternDisplayDigit3 = 0; // -
        patternDisplayDigit4 = 12; // C
        money -= 15;
        selectedDrink = "C";
      }
    }
  }

  displayDigits();
}

// Function to display the pattern for each digit
void displayDigits() {
    delay(displayDelay);
  DisplayPattern(1, patternDisplayDigit1);
    delay(displayDelay);
  DisplayPattern(2, patternDisplayDigit2);
    delay(displayDelay);
  DisplayPattern(3, patternDisplayDigit3);
    delay(displayDelay);
  DisplayPattern(4, patternDisplayDigit4);
}

// Function to choose the pattern to display
void DisplayPattern(int digit, int number) {
  digitalWrite(digit1, HIGH);
  digitalWrite(digit2, HIGH);
  digitalWrite(digit3, HIGH);
  digitalWrite(digit4, HIGH);
  
  switch(digit) {
    case 1:
      digitalWrite(digit1, LOW);
      break;
    case 2:
      digitalWrite(digit2, LOW);
      break;
    case 3:
      digitalWrite(digit3, LOW);
      break;
    case 4:
      digitalWrite(digit4, LOW);
      break;
  }
  
  int pattern = digitPatterns[number];
  digitalWrite(segmentA, bitRead(pattern, 0));
  digitalWrite(segmentB, bitRead(pattern, 1));
  digitalWrite(segmentC, bitRead(pattern, 2));
  digitalWrite(segmentD, bitRead(pattern, 3));
  digitalWrite(segmentE, bitRead(pattern, 4));
  digitalWrite(segmentF, bitRead(pattern, 5));
  digitalWrite(segmentG, bitRead(pattern, 6));

  delay(1); 
}
