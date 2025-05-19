#include <Adafruit_NeoPixel.h> // LED ring library
//#include "thingProperties.h" // IoT Cloud

#include <MQ135.h>
#include <DHT.h>

/* MQ135 + DHT Temp Sensor

   Combination of the MQ135 air quality sensor and a DHT11/22 temperature sensor to accurately measure ppm values through the library correction.
   Uses the Adafruit DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library

   Written by: https://github.com/NuclearPhoenixx/MQ135
*/

#define PIN_MQ135 A0 // MQ135 Analog Input Pin
#define DHTPIN D3 // DHT Digital Input Pin
#define DHTTYPE DHT11 // DHT11 or DHT22, depends on your sensor

MQ135 mq135_sensor(PIN_MQ135);
DHT dht(DHTPIN, DHTTYPE);

float temperature, humidity; // Temp and Humid floats, will be measured by the DHT

#define LED_PIN D1
#define LED_COUNT 12
#define LED_BRIGHTNESS 16 // 0 (min) to 255 (max)

#define BUTTON_BLACK_PIN D2
#define BUTTON_RED_PIN D5

#define DEFAULT_TICKRATE 50 // run loop every x ms
#define TIMER_DURATION 24000 // in milliseconds 1440000 = 24min
#define TIMER_ITERATIONS 3 // how many times the timer runs
#define PAUSE_DURATION 5000 // in milliseconds 300000 = 5min

#define TIMER_COLOR 92, 0, 0 // tomato
#define PAUSE_COLOR 22, 44, 139 // sky-blue


enum {
  BUTTON_BLACK,
  BUTTON_RED,
};

enum {   // states
  IDLE,  // waiting for input to start timer
  TIMER, // timer is running, display remaining time
  PAUSE, // pause timer shows other color
};

unsigned int state;

int tickRate = DEFAULT_TICKRATE;

bool buttonUp[2]; // track state of buttons

unsigned long timerStart; // save when the timer started
unsigned long timerDuration; // save how long the timer goes
int timerIteration; // save current iteration

// Declare our NeoPixel strip object:
Adafruit_NeoPixel ledRing = Adafruit_NeoPixel(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)


void setup() {
  // init dht sensor
  dht.begin();

  // init serial port for debug
  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
  delay(1500);
  Serial.println(F("Hello World"));

  

  return;

  // init led ring
  ledRing.begin();
  ledRing.fill();
  ledRing.setBrightness(LED_BRIGHTNESS);
  ledRing.show();

  // Connect to Arduino IoT Cloud
  //initProperties();
  //ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  //setDebugMessageLevel(2);
  //ArduinoCloud.printDebugInfo();

  // init buttons
  pinMode(BUTTON_RED_PIN, INPUT_PULLUP);
  pinMode(BUTTON_BLACK_PIN, INPUT_PULLUP);

  // startup animation
  //rainbowRingAnimation(2000);
}

void loop() {
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    delay(100);
    return;
  }

  float rzero = mq135_sensor.getRZero();
  float correctedRZero = mq135_sensor.getCorrectedRZero(temperature, humidity);
  float resistance = mq135_sensor.getResistance();
  float ppm = mq135_sensor.getPPM();
  float correctedPPM = mq135_sensor.getCorrectedPPM(temperature, humidity);

  Serial.print("C°: ");
  Serial.print(temperature);
  Serial.print(" hum: ");
  Serial.println(humidity);

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

  delay(300);

  //old
  return;
  // update arduino cloud
  //ArduinoCloud.update();

  // read button press
  bool blackPressed = wasButtonPressed(BUTTON_BLACK);
  bool redPressed = wasButtonPressed(BUTTON_RED);

  //DEBUG
  //Serial.println(F("Black: " + String(blackPressed)));
  //Serial.println(F("Red: " + String(redPressed)));
  
  // reset when both buttons are pressed
  if(!buttonUp[BUTTON_RED] && blackPressed || !buttonUp[BUTTON_BLACK] && redPressed) {
    reset();
    return;
  }

  switch (state) {
    case IDLE:
      if(redPressed || blackPressed) {
        //DEBUG
        //Serial.println(F("Idle -> Button pressed"));

        ledBlink(2, ledRing.Color(255, 0, 0));
        startTimer(TIMER_DURATION);
      }
      break;
    
    case TIMER:
      if(blackPressed) {
        //DEBUG
        //Serial.println(F("Timer -> Button pressed"));

        showCurrentIteration();
      }

      // check timer
      if (millis() - timerStart > timerDuration) {
        //Serial.println(F("Timer is up"));
        rainbowRingAnimation(2000);

        timerIteration++;

        // check if this was the fourth timer
        if (timerIteration >= TIMER_ITERATIONS) {
          // signal clear end
          ledBlink(5, ledRing.Color(255, 255, 255));

          //reset to idle
          reset();
          return;
        }

        startPause(PAUSE_DURATION);
      }

      updateRing();
      break;
    
    case PAUSE:
      if(redPressed || blackPressed) {
        //DEBUG
        //Serial.println(F("Pause -> Button pressed"));

        showCurrentIteration();
      }

      // check timer
      if (millis() - timerStart > timerDuration) {
        //Serial.println(F("Pause is up"));
        ledBlink(2, ledRing.Color(255, 0, 0));
        startTimer(TIMER_DURATION);
      }

      updateRing();
      break;
    
  }
  
  // delay next tick to save battery
  delay(tickRate);
}


/*
  Since Start is READ_WRITE variable, onStartChange() is
  executed every time a new value is received from IoT Cloud.
*/
//void onStartChange()  {
//  if (start && state == IDLE) {
//    // starts the timer when the start variable changes to true
//    startTimer(TIMER_DURATION);
//  }
//}


bool wasButtonPressed(int idx) {
  bool buttonDown = !digitalRead(idx == BUTTON_BLACK ? BUTTON_BLACK_PIN : BUTTON_RED_PIN);

  // button up currently
  if (!buttonDown) {
    buttonUp[idx] = true;
    return false;
  }

  // button is down currently
  if (buttonUp[idx]) {
    // button wasn't pressed last tick
    buttonUp[idx] = false;
    return true;
  }

  // but wasn't released yet
  return false;
}


// starts the timer with duration in seconds
void startTimer(int duration) {
  // save current elapsed milliseconds
  timerStart = millis();
  timerDuration = duration;
  state = TIMER;
}

void startPause(int duration) {
  timerStart = millis();
  timerDuration = duration;
  state = PAUSE;
}


void reset() {
  // reset state
  timerIteration = 0;
  state = IDLE;
  //start = false;

  // clear ring
  ledRing.fill();
  ledRing.show();

  // feedback
  ledBlink(2, ledRing.Color(0, 0, 255));
}


// blinks the led ring x times in given color
void ledBlink(int times, uint32_t color) {
  for (int i=0; i<times; i++) {
    ledRing.fill(color);
    ledRing.show();
    delay(200);
    ledRing.fill();
    ledRing.show();
    delay(200);
  }
}


void updateRing() {
  uint32_t color = 0;
  if (state == TIMER) {
    color = ledRing.Color(TIMER_COLOR);
  } else {
    color = ledRing.Color(PAUSE_COLOR);
  }

  float percentage = (float)(millis() - timerStart) / (float)timerDuration;
  int ringIndex = ceil((float)LED_COUNT * (1.0 - percentage));

  //Serial.println(percentage);

  for (int i=0; i < LED_COUNT; i++) {
    if (i < ringIndex) {
      ledRing.setPixelColor(i, color);

    } else {
      ledRing.setPixelColor(i, 0);

    }
  }

  ledRing.show();
}


// Seperates the ring in four segments and flashes the one corresponding to the current iteration
void showCurrentIteration() {
  int segmentSize = LED_COUNT / TIMER_ITERATIONS;
  int currentSegment = segmentSize * timerIteration;

  ledRing.fill();
  
  for (int i=0; i < currentSegment; i++) {
    ledRing.setPixelColor(i, ledRing.Color(255, 255, 255));
  }
  ledRing.show();

  for (int j=0; j < 3; j++) {
    for (int i=0; i < segmentSize; i++) {
      if (j % 2) {
        ledRing.setPixelColor(currentSegment + i, 0);

      } else {
        ledRing.setPixelColor(currentSegment + i, ledRing.Color(0, 255, 0));

      }
    }

    ledRing.show();
    delay(300);
  }
}


// Fills and empties ring with a rainbow
void rainbowRingAnimation(unsigned long duration) {
  int fraction = 255 / LED_COUNT;
  int wait = duration / 2 / LED_COUNT;

  // wipe in
  for(int i=0; i < LED_COUNT; i++) {
    ledRing.setPixelColor(i, colorWheel(i * fraction));
    ledRing.show();
    delay(wait);
  }
  
  // wipe out
  for(int i=0; i < LED_COUNT; i++) {
    ledRing.setPixelColor(i, 0);
    ledRing.show();
    delay(wait);
  }
}


// CC adafruit examples
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t colorWheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return ledRing.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return ledRing.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return ledRing.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
