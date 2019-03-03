#include <gfx.h>
#include "temperature/temperature.h"

font_t font = gdispOpenFont("DejaVuSans32");
TemperatureSensor temperatureSensor;

void setup() {
  gCoord		width, height;

  Serial.begin(115200);
  delay(10);

  // Initialize and clear the display
  Serial.println("gfxInit");
  gfxInit();
  Serial.println("gfxInit done");

  // Get the screen size
  width = gdispGetWidth();
  Serial.printf("width %d\n", width);
  height = gdispGetHeight();
  Serial.printf("height %d", height);
  Serial.println();

  unsigned displayCount = gdispGetDisplayCount();
  Serial.printf("num displays %d", displayCount);
  Serial.println();

  GDisplay* display = gdispGetDisplay(0);

  gPowermode powerMode = gdispGGetPowerMode(display);
  Serial.printf("powermode: %d", powerMode);
  Serial.println();

  // gdispSetPowerMode(gPowerDeepSleep);
  // powerMode = gdispGGetPowerMode(display);
  // Serial.printf("powermode: %d", powerMode);
  // Serial.println();

  // gdispSetPowerMode(gPowerOn);
  // powerMode = gdispGGetPowerMode(display);
  // Serial.printf("powermode: %d", powerMode);
  // Serial.println();

  // Code Here
  Serial.println("clear screen");
  // gdispClear(GFX_WHITE);
  // // gdispFillArea(width / 2, height / 2, width / 2 - 10, height / 2 - 10, GFX_BLACK);
  // font_t font = gdispOpenFont("DejaVuSans16");
  // gdispDrawString(10, height / 3, "Dies ist ein Test!", font, GFX_BLACK);
  // gdispCloseFont(font);
  // // for (i = 5, j = 0; i < width && j < height; i += 7, j += i / 20)
  // //   gdispDrawPixel(i, j, GFX_WHITE);

  // Serial.println("flush screen");
  // gdispFlush();

  // // Serial.println("sleep");
  // // gdispSetPowerMode(gPowerDeepSleep);

  // powerMode = gdispGetPowerMode();
  // Serial.printf("powermode: %d", powerMode);
  // Serial.println();
}

void loop() {
  // gPowermode powerMode = gdispGetPowerMode();
  // Serial.printf("powermode: %d", powerMode);
  // Serial.println();
  // put your main code here, to run repeatedly:
  
  TempAndHumidity readValues = temperatureSensor.getTempAndHumidity();

  gCoord width, height;
  width = gdispGetWidth();
  height = gdispGetHeight();

  gdispClear(GFX_WHITE);
  gdispDrawString(width / 4, height / 3, String("Temperatur: " + String(readValues.temperature)).c_str(), font, GFX_BLACK);
  gdispDrawString(width / 4, 2 * height / 3, String("Feuchtigkeit: " + String(readValues.humidity)).c_str(), font, GFX_BLACK);
  gdispFlush();

  delay(120000);
}

