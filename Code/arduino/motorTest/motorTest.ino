#include <AccelStepper.h>

// Stabilere Pins (vermeide GPIO0 und GPIO15)
#define IN1 D5  // GPIO14
#define IN2 D6  // GPIO12
#define IN3 D7  // GPIO13
#define IN4 D2  // GPIO4

AccelStepper stepper(AccelStepper::HALF4WIRE, IN1, IN3, IN2, IN4);

void setup() {
  Serial.begin(115200);
  Serial.println("Stepper-Test (stabil)");

  stepper.setMaxSpeed(500);
  stepper.setAcceleration(200);
  stepper.setSpeed(300);
}

void loop() {
  // 512 Schritte vorwärts (ohne delay!)
  stepper.moveTo(512);
  while (stepper.run()) {
    yield(); // Verhindert Watchdog-Reset
  }

  // 512 Schritte rückwärts
  stepper.moveTo(0);
  while (stepper.run()) {
    yield();
  }
}