#define USER_SETUP_LOADED
#include "usersetup.h"

#include <TFT_eSPI.h>
#include <SPI.h>




TFT_eSPI tft = TFT_eSPI(); // Display-Objekt

int centerX, centerY, radius;

void setup() {
  tft.init();
  tft.setRotation(0);  // ggf. anpassen (0-3)
  tft.fillScreen(TFT_WHITE);

  centerX = tft.width() / 2;
  centerY = tft.height() / 2;
  radius = min(centerX, centerY) - 10;

  drawClockFace();

  // Testzeit: 10:10:30
  drawHands(10, 10, 30);
}

void loop() {
  // nichts – statisch
}

void drawClockFace() {
  tft.drawCircle(centerX, centerY, radius, TFT_WHITE);

  for (int i = 0; i < 12; i++) {
    float angle = i * 30 * DEG_TO_RAD;
    int x1 = centerX + cos(angle) * (radius - 4);
    int y1 = centerY + sin(angle) * (radius - 4);
    int x2 = centerX + cos(angle) * (radius - 10);
    int y2 = centerY + sin(angle) * (radius - 10);
    tft.drawLine(x1, y1, x2, y2, TFT_WHITE);
  }

  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(centerX - 30, centerY - radius - 20);
  tft.print("Test Uhr");
}

void drawHands(int h, int m, int s) {
  float secAngle = s * 6;
  float minAngle = m * 6;
  float hourAngle = h * 30 + m * 0.5;

  drawHand(hourAngle, radius * 0.5, TFT_WHITE);
  drawHand(minAngle, radius * 0.7, TFT_CYAN);
  drawHand(secAngle, radius * 0.9, TFT_RED);
}

void drawHand(float angleDeg, int length, uint16_t color) {
  float angle = angleDeg * DEG_TO_RAD - PI / 2;
  int x = centerX + cos(angle) * length;
  int y = centerY + sin(angle) * length;
  tft.drawLine(centerX, centerY, x, y, color);
}
