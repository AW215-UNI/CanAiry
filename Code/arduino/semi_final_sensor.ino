#include <Adafruit_NeoPixel.h>
#include <DHT.h> // Libs used
#include "thingProperties.h"

// Pins defenition
#define DHTPIN D4
#define DHTTYPE DHT11 

#define LED_PIN D2
#define STRIPSIZE 24

#define BUTTON_ONE D1
boolean isLedOn = false;
boolean buttonUp = false;

float sensor = A0;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(STRIPSIZE, LED_PIN, NEO_GRB + NEO_KHZ800);

float gas_value; 

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // Initialize serial and wait for port to open:
  pinMode(sensor,INPUT); 
  pinMode(BUTTON_ONE, INPUT_PULLUP);
  Serial.begin(9600);
  // init dht sensor
  dht.begin();

  delay(1500);
  Serial.println(F("Initializing sensors..."));

  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  delay(1500); 

  strip.begin();  // Only works if board is connected to Internet
  strip.setBrightness(50);
  strip.show();  // Initialize all pixels to off

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
  

  // Check if any reads failed and exit early (to try again).
  if (isnan(feuchtigkeit) || isnan(temperatur)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    delay(100);
    return;
  }

  // Read air quality data
gas_value=analogRead(sensor);
kohli = gas_value;

// Print data
  
Serial.println(String(F("CO2 Value: ")) + gas_value);
  
Serial.println(String(F("Humidity Value: ")) + feuchtigkeit); 

Serial.println(String(F("Temperatur Value: ")) + temperatur);

  if(wasButtonPressed()) {
    isLedOn = !isLedOn;
  }
    
  if (isLedOn) {
          for (int i = 0; i <= STRIPSIZE; i++) {  // Turn Loop LED off
      strip.setPixelColor(i, strip.Color(0, 0, 0));
      strip.show();
    }
    return;
  }


  if (gas_value < 600) { // good CO2 value
      for (int i = 0; i <= STRIPSIZE; i++) {  // Turn Loop LED on
      strip.setPixelColor(i, strip.Color(0, 255, 0));
      strip.show();
    }
  }

    if (gas_value > 600 && gas_value < 700 ) { // medium CO2 value
      for (int i = 0; i <= STRIPSIZE; i++) {  // Turn Loop LED on
      strip.setPixelColor(i, strip.Color(90, 255, 0));
      strip.show();
    }
  }
  
    if (gas_value > 700 && gas_value < 800) { // acceptable CO2 value
      for (int i = 0; i <= STRIPSIZE; i++) {  // Turn Loop LED on
      strip.setPixelColor(i, strip.Color(125, 125, 0));
      strip.show();
    }
  }
      if (gas_value > 800) { // bad CO2 value
      for (int i = 0; i <= STRIPSIZE; i++) {  // Turn Loop LED on
      strip.setPixelColor(i, strip.Color(0, 0, 0));
      strip.show();
    }
        delay(1000);
       for (int i = 0; i <= STRIPSIZE; i++) {  // Turn Loop off
      strip.setPixelColor(i, strip.Color(255, 0, 0));
      strip.show(); 
  }
      }
}
/*
  Since CO2 is READ_WRITE variable, onCO2Change() is
  executed every time a new value is received from IoT Cloud.
*/
void onCO2Change()  {
  // Add your code here to act upon CO2 change
}

bool wasButtonPressed() {
  bool buttonDown = !digitalRead(BUTTON_ONE);

  // button up currently
  if (!buttonDown) {
    buttonUp = true;
    return false;
  }

  // button is down currently
  if (buttonUp) {
    // button wasn't pressed last tick
    buttonUp = false;
    return true;
  }

  // but wasn't released yet
  return false;
}
