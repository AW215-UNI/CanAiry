#include <Adafruit_NeoPixel.h> // LED ring library

#define BUTTON_ONE D1
/*#define BUTTON_TWO D6*/

#define DEFAULT_TICKRATE 50 // run loop every x ms
#define TIMER_DURATION 24000 // in milliseconds 1440000 = 24min
#define TIMER_ITERATIONS 3 // how many times the timer runs
#define PAUSE_DURATION 5000 // in milliseconds 300000 = 5min






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
//Adafruit_NeoPixel ledRing = Adafruit_NeoPixel(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
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
  //dht.begin();

  // init serial port for debug
  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
  delay(1500);
  Serial.println(F("Hello World"));

  pinMode(BUTTON_ONE, INPUT_PULLUP);
  //pinMode(BUTTON_TWO, INPUT_PULLUP);

  return;

  // init led ring
  /*ledRing.begin();
  ledRing.fill();
  ledRing.setBrightness(LED_BRIGHTNESS);
  ledRing.show();*/

  // Connect to Arduino IoT Cloud
  //initProperties();
  //ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  //setDebugMessageLevel(2);
  //ArduinoCloud.printDebugInfo();

  // init buttons
//  pinMode(BUTTON_RED_PIN, INPUT_PULLUP);
  //pinMode(BUTTON_BLACK_PIN, INPUT_PULLUP);

  // startup animation
  //rainbowRingAnimation(2000);
}

void loop() {
 // humidity = dht.readHumidity();
 // temperature = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  /*if (isnan(humidity) || isnan(temperature)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    delay(100);
    return;
  }

  float rzero = mq135_sensor.getRZero();
  float correctedRZero = mq135_sensor.getCorrectedRZero(temperature, humidity);
  float resistance = mq135_sensor.getResistance();
  float ppm = mq135_sensor.getPPM();
  float correctedPPM = mq135_sensor.getCorrectedPPM(temperature, humidity);
*/


  /*if (!digitalRead(BUTTON_ONE) && !digitalRead(BUTTON_TWO)) {
    Serial.println("Hello World!");
    return;*/

  if(!digitalRead(BUTTON_ONE)) {
    Serial.println("Hello All!");
  }

  /*if(!digitalRead(BUTTON_TWO)) {
    Serial.println("All!");
  }*/

  delay(300);

  //old
  return;

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









  


