long int TIME = 1000;

int pot = 0;
int on = 0;
int off = 0;
int let = 7;

void setup() {
  Serial.begin(9600);
  pinMode(let, OUTPUT);
}

void loop() {
  pot = analogRead(A0);
  on = timeOn(pot);
  off = TIME - on;
  
  digitalWrite(let, HIGH);
  delay(on);
  digitalWrite(let, LOW);
  delay(off);
}

int timeOn(int pot) {
  return (float)pot * TIME / 1023;
}
