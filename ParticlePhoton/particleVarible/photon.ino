// This #include statement was automatically added by the Particle IDE.
#include <Adafruit_Sensor.h>

// This #include statement was automatically added by the Particle IDE.
#include <DHT.h>


SYSTEM_THREAD(ENABLED);

#define DHTPIN 2 // what digital pin we&#39;re connected to
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

double t; // temperatura en °C
double f; // temperatura en °F
double h; // porcentaje de hunedad
double hic; //indicce calor

void setup() {

  Serial.begin(9600);
  dht.begin();
  Particle.variable("TEMP",t); // se envia al servidor de particle la temperatura dada por el sensor
  Particle.variable("HUM", h); // se envia al servidor de particle la humedad dada por el sensor

}

void loop() {

  // Wait a few seconds between measurements.
  // Sensor readings may also be up to 2 seconds &#39;old&#39; (its a very slow sensor)
  h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  f = dht.readTemperature(true);
  hic = dht.computeHeatIndex(t, h, false);

}


