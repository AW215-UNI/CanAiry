#include <DHT.h>
#include <MQ135.h>
#include "thingProperties.h"

#define DHTPIN D3 // Define the pins
#define DHTTYPE DHT11 
float sensor = A0;

float gas_value; 

MQ135 mq135_sensor(PIN_MQ135);
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // Initialize serial and wait for port to open:
  pinMode(sensor,INPUT); 
  Serial.begin(9600);
  // init dht sensor
  dht.begin();

  delay(1500);
  Serial.println(F("Initializing sensors..."));

  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  delay(1500); 

  // Defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}

void loop() {
  ArduinoCloud.update();
  
  // Read humidity and temperature from DHT sensor
  feuchtigkeit = dht.readHumidity(); // reads humidity, temp, co2
  temperatur = dht.readTemperature();
  cO2 = gas_value;
  

  // Check if any reads failed and exit early (to try again).
  if (isnan(feuchtigkeit) || isnan(temperatur)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    delay(100);
    return;
  }

  // Read air quality data
gas_value=analogRead(sensor); 


Serial.println(gas_value);

Serial.println(feuchtigkeit); 

Serial.println(temperatur); 


  // Print sensor data


  delay(3000); // Wait 3 seconds between measurements
}