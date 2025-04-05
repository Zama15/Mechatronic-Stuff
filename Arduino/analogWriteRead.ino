long int TIME = 10;

const int pot = A0;
const int led = 7;

int off = 0;
int on = 0;
int potOutput = 0;

void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);
}

void loop() {
  potOutput = analogRead(pot);
  on = timeOn(potOutput);
  off = TIME - on;
  // Serial.println("Pot output: " + String(potOutput) + " | On: " + String(on) + " | Off: " + String(off));
  
  if (on > 0) {
    digitalWrite(led, HIGH);
    delay(on);
  }
  digitalWrite(led, LOW);
  delay(off);
}

int timeOn(int pot) {
  int pulse = pot * TIME / 1023;
  // Serial.println("Pulse: " + String(pot) + " * " + String(TIME) + " / 1023 = " + String(pulse));
  return pulse;
}
