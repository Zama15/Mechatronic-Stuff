// This #include statement was automatically added by the Particle IDE.
#include <DHT.h>

// This #include statement was automatically added by the Particle IDE.
#include <Adafruit_Sensor.h>

#include "math.h"  // libreria para funciones matematicas
#include "stdio.h" // libreria estándar de entradas salidas

SYSTEM_THREAD(ENABLED); // For running with/without internet conection

#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
double temperature;
double humidity;

double max_temp = 20;
double max_hum = 40;

int led = D0;

double temperatureFunc(String command) {
  max_temp = atof(command);
  return max_temp;
}

double humidityFunc(String command) {
  max_hum = atof(command);
  return max_hum;
}

void setup() {
  pinMode(led, OUTPUT);
  dht.begin();
  Particle.function("setMaxTemp", temperatureFunc);
  Particle.function("setMaxHum", humidityFunc);
  Particle.variable("TEMP", temperature);
  Particle.variable("HUM", humidity);
  Particle.variable("MAX_TEMP", max_temp);
  Particle.variable("MAX_HUM", max_hum);
}

void loop() {
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
  
  if (isnan(temperature) || isnan(humidity)) {
    Particle.publish("Error", "Failed to read from DHT sensor!", PRIVATE);
    return;
  }

  if (temperature > max_temp || humidity > max_hum) {
    digitalWrite(led, HIGH);
    Particle.publish("Temperature", String(temperature), PRIVATE);
    Particle.publish("Humidity", String(humidity), PRIVATE);
  } else {
    digitalWrite(led, LOW);
  }
  
  delay(2000);
}

