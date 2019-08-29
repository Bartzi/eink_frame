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


#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define S_TO_MIN_FACTOR 60
#define TIME_TO_SLEEP  60        /* Time ESP32 will go to sleep (in units) */
#define TIME_TO_SLEEP_BETWEEN_FAILURE 1

font_t font = gdispOpenFont("DejaVuSans32");
TemperatureSensor temperatureSensor;

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

String cityId = "2852458";  // Potsdam


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

void print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
  }
}

void gotoSleep(uint sleepTime) {
  print_wakeup_reason();
  esp_sleep_enable_timer_wakeup(sleepTime * uS_TO_S_FACTOR * S_TO_MIN_FACTOR);
  Serial.println("Setup ESP32 to sleep for every " + String(sleepTime) +
  " minutes");
  Serial.println("Going to sleep now");
  Serial.flush(); 
  esp_deep_sleep_start();
}

void setup() {
  Serial.begin(115200);
  delay(10);

  Serial.println(ESP.getFreeHeap());
  Serial.println("gfxInit");
  gfxInit();
  Serial.println("gfxInit done");
  Serial.println(ESP.getFreeHeap());

  bool connectionSuccessful = connectToWifi(true);
  Serial.println(ESP.getFreeHeap());

  GDisplay* display = gdispGetDisplay(0);
  WeatherUI ui = WeatherUI(cityId, display);
  auto heapBefore = ESP.getFreeHeap();

  uint sleepTime = TIME_TO_SLEEP;
  if (connectionSuccessful) {
    ui.updateForecast();
  } else {
    sleepTime = TIME_TO_SLEEP_BETWEEN_FAILURE;
    ui.showConnectionError(String(ssid), String(TIME_TO_SLEEP_BETWEEN_FAILURE));
  }

  auto heapAfter = ESP.getFreeHeap();
  Serial.printf("Heap: Before - After: %d\n", heapBefore - heapAfter);

  gPowermode powerMode = gdispGetPowerMode();
  Serial.printf("powermode: %d", powerMode);
  gotoSleep(sleepTime);
}

void loop() {
}

