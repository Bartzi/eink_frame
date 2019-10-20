#include <cstring>
#include <vector>
#include <WiFi.h>

extern "C" {
  #include "gfx.h"
}

#include "sleep/time_management.h"
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

  bool hasNoError = connectToWifi(true);
  int errorSleepMinutes = 1;

  Serial.println(ESP.getFreeHeap());

  // create default error message
  String errorMessage = String(F("Could not connect to WiFi!"));
  String errorExtraInfo = String(ssid);

  // get the current forecast
  AccuWeatherAPI api(cityId);
  std::vector<WeatherData> weatherInfo;
  String message = api.fetchForecast(weatherInfo);
  if (message.length() > 0) {
    hasNoError = false;
    errorSleepMinutes = 60;
    errorMessage = message;
    errorExtraInfo = String(F(""));
  }

  // init GUI
  Serial.println(ESP.getFreeHeap());
  Serial.println("gfxInit");
  gfxInit();
  Serial.println("gfxInit done");
  Serial.println(ESP.getFreeHeap());

  GDisplay* display = gdispGetDisplay(0);
  AccuWeatherUI ui(display);
  TimeManager timeManager(display);
  timeManager.showTimeStamp();
  
  auto heapBefore = ESP.getFreeHeap();

  // display forecast or error
  if (hasNoError) {
    ui.updateForecast(weatherInfo, &api);
  } else {
    ui.showErrorMessage(errorMessage, errorExtraInfo, String(errorSleepMinutes));
  }

  auto heapAfter = ESP.getFreeHeap();
  Serial.printf("Heap: Before - After: %d\n", heapBefore - heapAfter);

  gPowermode powerMode = gdispGetPowerMode();
  Serial.printf("powermode: %d", powerMode);
  timeManager.gotoSleep(!hasNoError, errorSleepMinutes);
}

void loop() {
}
