#include <cstring>
#include <vector>
#include <WiFi.h>

extern "C" {
  #include "gfx.h"
}

#include "temperature/temperature.h"
#include "weather/weather_api.h"
#include "weather_ui/weather_ui.h"
#include "wifi_credentials.h"


font_t font = gdispOpenFont("DejaVuSans32");
TemperatureSensor temperatureSensor;

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

String cityId = "2852458";  // Potsdam


void connectToWifi() {
  WiFi.mode(WIFI_STA);
  delay(2000);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected!");
}

void setup() {
  Serial.begin(115200);
  delay(10);

  Serial.println(ESP.getFreeHeap());
  Serial.println("gfxInit");
  gfxInit();
  Serial.println("gfxInit done");
  Serial.println(ESP.getFreeHeap());

  // fix display memory

  connectToWifi();
  Serial.println(ESP.getFreeHeap());

  WeatherUI ui = WeatherUI(cityId, gdispGetDisplay(0));
  auto heapBefore = ESP.getFreeHeap();
  ui.updateForecast();
  auto heapAfter = ESP.getFreeHeap();
  
  Serial.printf("Heap: Before - After: %d\n", heapBefore - heapAfter);

  gPowermode powerMode = gdispGetPowerMode();
  Serial.printf("powermode: %d", powerMode);
}

void loop() {
  // // gPowermode powerMode = gdispGetPowerMode();
  // // Serial.printf("powermode: %d", powerMode);
  // // Serial.println();
  // // put your main code here, to run repeatedly:
  
  // TempAndHumidity readValues = temperatureSensor.getTempAndHumidity();

  // gCoord width, height;
  // width = gdispGetWidth();
  // height = gdispGetHeight();

  // gdispClear(GFX_WHITE);
  // gdispDrawString(width / 4, height / 3, String("Temperatur: " + String(readValues.temperature)).c_str(), font, GFX_BLACK);
  // gdispDrawString(width / 4, 2 * height / 3, String("Feuchtigkeit: " + String(readValues.humidity)).c_str(), font, GFX_BLACK);
  // gdispFlush();

  delay(100);
}

