#include <DHT.h>
#include <MQ135.h>
#include "thingProperties.h"

#define PIN_MQ135 A0
#define DHTPIN D3 
#define DHTTYPE DHT11 

MQ135 mq135_sensor(PIN_MQ135);
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // Initialize serial and wait for port to open:
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
  cO2 = mq135_sensor.getCorrectedPPM(temperatur, feuchtigkeit);
  

  // Check if any reads failed and exit early (to try again).
  if (isnan(feuchtigkeit) || isnan(temperatur)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    delay(100);
    return;
  }

  // Read air quality data
  float rzero = mq135_sensor.getRZero();
  float correctedRZero = mq135_sensor.getCorrectedRZero(temperatur, feuchtigkeit);
  float resistance = mq135_sensor.getResistance();
  float ppm = mq135_sensor.getPPM();
  float correctedPPM = mq135_sensor.getCorrectedPPM(temperatur, feuchtigkeit);



  // Print sensor data
  Serial.print("Temperature: ");
  Serial.print(temperatur);
  Serial.print("°C, Humidity: ");
  Serial.print(feuchtigkeit);
  Serial.println("%");

  Serial.print("MQ135 RZero: ");
  Serial.print(rzero);
  Serial.print("\t Corrected RZero: ");
  Serial.print(correctedRZero);
  Serial.print("\t Resistance: ");
  Serial.print(resistance);
  Serial.print("\t PPM: ");
  Serial.print(ppm);
  Serial.print("ppm");
  Serial.print("\t Corrected PPM: ");
  Serial.print(correctedPPM);
  Serial.println("ppm");

  delay(3000); // Wait 3 seconds between measurements
}