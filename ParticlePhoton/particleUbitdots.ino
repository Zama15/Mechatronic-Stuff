#include <DHT.h>
#include <Ubidots.h>
#include <Adafruit_Sensor.h>

SYSTEM_THREAD(ENABLED); // for running with/without internet connection

#ifndef TOKEN
#define TOKEN "" // Put here your Ubidots TOKEN
#endif

Ubidots ubidots(TOKEN, UBI_TCP);

#include "math.h"
#include "stdio.h"

SYSTEM_THREAD(ENABLED);

#define DHTPIN 2 // what digital pin we're connected to
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

double t; // temperatura en °C
double f; // temperatura en °F
double h; // porcentaje de humedad
double hic; //indice calor

void setup() {
    Serial.begin(9600);
    dht.begin();
}


void loop() {
    
    // Wait a few seconds between measurements.
    delay(10000); // se interroga al sensor cada 10 segundos
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    
    h = dht.readHumidity();
    t = dht.readTemperature();
    f = dht.readTemperature(true);
    hic = dht.computeHeatIndex(t, h, false);
    
    ubidots.add("TEMPC", t);
    ubidots.add("HUM", h);
    ubidots.add("TEMPF", f);
    ubidots.add("HIC", hic);
    bool bufferSent = false;
    bufferSent = ubidots.send();
}

