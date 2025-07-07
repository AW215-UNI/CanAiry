#include <Arduino_GFX_Library.h>

// Pin-Definitionen für SPI-Ansteuerung
#define TFT_CS     15   // D8
#define TFT_DC     0    // D3
#define TFT_RST    2    // D4

// Hardware SPI verwenden (MOSI=GPIO13, SCK=GPIO14)
Arduino_DataBus *bus = new Arduino_ESP8266SPI(TFT_DC, TFT_CS, TFT_CLK, TFT_DA);
Arduino_GFX *gfx = new Arduino_GC9A01(bus, TFT_RST, true);

void setup() {
  Serial.begin(115200);
  gfx->begin();
  gfx->fillScreen(WHITE);

  gfx->setTextColor(GREEN);
  gfx->setTextSize(2);
  gfx->setCursor(40, 120);
  gfx->println("GC9A01 läuft!");
}

void loop() {
  // nichts
}
