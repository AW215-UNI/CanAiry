#include <DHT.h> // Libs used
#include "thingProperties.h"
// Pins defenition
#define DHTPIN D3 
#define DHTTYPE DHT11 
float sensor = A0;

float gas_value; 

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

// Print data
  
Serial.println(String(F("CO2 Value: ")) + gas_value);
  
Serial.println(String(F("Humidity Value: ")) + feuchtigkeit); 

Serial.println(String(F("Temperatur Value: ")) + temperatur); 



  delay(3000); // Wait 3 seconds between measurements
}