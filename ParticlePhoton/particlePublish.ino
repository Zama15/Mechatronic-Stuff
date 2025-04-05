// This #include statement was automatically added by the Particle IDE.
#include <DHT.h>

// This #include statement was automatically added by the Particle IDE.
#include <Adafruit_Sensor.h>

// Ejecuta el programa haya o no haya internet
SYSTEM_THREAD(ENABLED);

#define DHTPIN 2     // what digital pin we're connected to
#define DHTTYPE DHT11   

DHT dht(DHTPIN, DHTTYPE);
double t; // temperatura en °C
double f; // temperatura en °F
double h; // porcentaje de hunedad
double hic; //indicce calor
int led1 = D7; // This one is the little blue LED on your board. On the Photon it is next to D7, and on the Core it is next to the USB jack.



void setup() {
  pinMode(led1, OUTPUT);

  Serial.begin(9600);
  dht.begin(); // Se inicializa la lectura del sensor

}

//}
void loop() {

  // Wait a few seconds between measurements.
  delay(5000); // se interroga al sensor cada 5 segundos

  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  f = dht.readTemperature(true);

  hic = dht.computeHeatIndex(t, h, false);

  // se verifica que la temperatura ambiente 
  if (t>24) { // si es mayor enciende el dispositivo de control
    digitalWrite(led1, HIGH);
  } else {  // en caso contrario se apaga el dispositivo de control
    digitalWrite(led1, LOW);
  }
  String TEMP = String(t); // se convierte a cadena de caracteres el valor numérico del valor t
  String H = String(h); // se convierte a cadena de caracteres el valor numérico del valor h

  Particle.publish("Temp °C",TEMP, PRIVATE); // se publica el valor en la plataforma de Particle con el nombre de “TEMP, puede ser con el nombre que ud desee
  Particle.publish("% Humedad",H, PRIVATE); // se publica el valor en la plataforma de Particle con el nombre de “H, puede ser con el nombre que ud desee

} 

