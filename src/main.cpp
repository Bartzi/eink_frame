#include <cstring>
#include <vector>
#include <WiFi.h>

extern "C" {
  #include "gfx.h"
}

#include "sleep/sleep.h"
#include "temperature/temperature.h"
#include "weather/accuweather_api.h"
#include "weather_ui/accuweather_ui.h"
// #include "weather_ui/openweathermap_ui.h"
#include "wifi_credentials.h"

#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"


font_t font = gdispOpenFont("DejaVuSans32");
TemperatureSensor temperatureSensor;

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

// String cityId = "2852458";  // Potsdam
String cityId = "167904";  // Potsdam


bool connectToWifi(bool withPassword) {
  Serial.println("Connecting to " + String(ssid));
  WiFi.mode(WIFI_STA);
  delay(2000);
  if (withPassword) {
    WiFi.begin(ssid, password);
  } else {
    WiFi.begin(ssid);
  }
  
  uint counter = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    if (counter > 100) {
      return false;
    }
    counter++;
  }
  Serial.println("Connected!");
  return true;
}

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
  Serial.begin(115200);
  delay(10);

  bool connectionSuccessful = connectToWifi(true);
  Serial.println(ESP.getFreeHeap());

  AccuWeatherAPI api(cityId);
  std::vector<WeatherData> weatherInfo;
  api.fetchForecast(weatherInfo);

  Serial.println(ESP.getFreeHeap());
  Serial.println("gfxInit");
  gfxInit();
  Serial.println("gfxInit done");
  Serial.println(ESP.getFreeHeap());

  GDisplay* display = gdispGetDisplay(0);
  AccuWeatherUI ui(display);
  auto heapBefore = ESP.getFreeHeap();

  bool errorSleep = false;
  if (connectionSuccessful) {
    ui.updateForecast(weatherInfo, &api);
  } else {
    errorSleep = true;
    ui.showConnectionError(String(ssid), String(TIME_TO_SLEEP_BETWEEN_FAILURE));
  }

  auto heapAfter = ESP.getFreeHeap();
  Serial.printf("Heap: Before - After: %d\n", heapBefore - heapAfter);

  gPowermode powerMode = gdispGetPowerMode();
  Serial.printf("powermode: %d", powerMode);
  gotoSleep(errorSleep);
}

void loop() {
}

