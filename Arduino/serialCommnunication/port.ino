const long int TIME = 10;

const int pot = A0;
const int led = 7;

int potOutput;
char serialInput;

long int tempo = 0;
long int t1 = 0;


void setup() {
  Serial.begin(9600);
  pinMode(led, OUTPUT);
}

void loop() {
  tempo = millis();
  potOutput = analogRead(pot);

  if (Serial.available() > 0) {
    serialInput = Serial.read();
  }

  switch (serialInput) {
    case '1':
      digitalWrite(led, HIGH);
      break;
    case '0':
      digitalWrite(led, LOW);
      break;
  }

  if ((tempo - t1) >= 500) {
    t1 = tempo;
    int on = timeOn(potOutput);
    int off = TIME - on;
    Serial.println(String(on) + "/" + String(off) + "/" + String(TIME) + "/" + String(potOutput));
  }
}

int timeOn(int pot) {
  int pulse = pot * TIME / 1023;
  return pulse;
}
