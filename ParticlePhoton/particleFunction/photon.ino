int ledPin0 = D0;
int ledPin1 = D1;
int ledPin2 = D2;
int ledPin3 = D3;
int ledPin4 = D4;
int ledPin5 = D5;
int ledPin6 = D6;
int ledPin7 = D7;

int pinSelected = D0;
int pinState = LOW;

int myFunction(String command) {
  Particle.publish("Debug", "myFunction llamada con: " + command, PRIVATE);
  return 1; 
}

int controlarLedStado(String comando) {
  if (comando == "on") {
    pinState = HIGH;

    return 1; // Éxito
  } else if (comando == "off") {
    pinState = LOW;

    return 0; // Éxito
  }

  return -1; // Error
}

int controlarLed(String comando) {
  if (comando == "1") {
    pinSelected = D1;

    return 1; // Éxito
  } else if (comando == "2") {
    pinSelected = D2;

    return 2; // Éxito
  } else if (comando == "3") {
    pinSelected = D3;

    return 3; // Éxito
  } else if (comando == "4") {
    pinSelected = D4;

    return 4; // Éxito
  } else if (comando == "5") {
    pinSelected = D5;

    return 5; // Éxito
  } else if (comando == "6") {
    pinSelected = D6;

    return 6; // Éxito
  } else if (comando == "7") {
    pinSelected = D7;

    return 7; // Éxito
  } else if (comando == "0") {
    pinSelected = D0;

    return 8; // Éxito
  }
  return -1; // Error
}

void setup() {
  pinMode(ledPin0, OUTPUT);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(ledPin4, OUTPUT);
  pinMode(ledPin5, OUTPUT);
  pinMode(ledPin6, OUTPUT);
  pinMode(ledPin7, OUTPUT);

  Particle.function("pin", controlarLed);
  Particle.function("state", controlarLedStado);

  Serial.begin(9600);
}

void loop() {
  digitalWrite(pinSelected, pinState);
  Particle.publish("Pin: ",String(pinSelected), PRIVATE);
  Particle.publish("State: ",pinState == HIGH ? "ON" : "OFF", PRIVATE);

  // Particle.function("pin", controlarLed);
  // Particle.function("state", controlarLedStado);
}

