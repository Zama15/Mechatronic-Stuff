// This #include statement was automatically added by the Particle IDE.
#include <Adafruit_Sensor.h>

// This #include statement was automatically added by the Particle IDE.
#include <DHT.h>

SYSTEM_THREAD(ENABLED);

#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

double h;
double t;

String to = "+523141000104";

String makeXML(double h, double t, String voiceGender = "woman") {
    String temperature = String(t);
    String humidity = String(h);
    // <Response>
    //  <Say voice="voiceGender">
    //     ALERT!!!
    //     The temperature in the area exceeded t degrees Celsius.
    //     The humidity of the area is h percent.
    //  </Say>
    // </Response>
    String prefix = "<Response><Say voice='" + voiceGender + ">ALERT!!! ";
    String body1 = "The temperature in the area exceeded " + temperature + " degrees Celsius. ";
    String body2 = "The humidity of the area is " + humidity + " percent.";
    String suffix = "</Say></Response>";
    
    return prefix + body1 + body2 + suffix;
}

void twilioVoice(String to, String xml) {
    // {"to": "to", "xml": "xml"}
    String json = "{\"to\": \"" + to + "\", \"twiml\": \"" + xml + "\"}";
    Particle.publish("twilioVoice", json, PRIVATE);
}

void setup() {
    Serial.begin(9600);
    dht.begin();
}

void loop() {
    h = dht.readHumidity();
    t = dht.readTemperature();
    
    if (t > 25) {
        twilioVoice(to, makeXML(h, t));
    }
    
    Particle.publish("Temp °C", String(t), PRIVATE);
    Particle.publish("% Humedad", String(h), PRIVATE);
    
    delay(60000);
}


