#include <cstring>
#include <vector>
#include <WiFi.h>

extern "C" {
  #include "gfx.h"
}

#include "temperature/temperature.h"
#include "weather/weather_api.h"
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

static gdispImage image;

void setup() {
  gCoord		width, height;

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

  WeatherAPI api = WeatherAPI(cityId);
  std::vector<WeatherData> weatherInfo;
  api.fetchForecast(weatherInfo);

  std::unique_ptr<uint8_t[]> icon;
  try {
    icon = api.fetchWeatherIcon(weatherInfo[0].icon);
  } catch (const std::logic_error &e) {
    Serial.println(F("Could not fetch weather icon"));
    Serial.println(e.what());
    return;
  }

  GFILE* imageData = gfileOpenMemory(reinterpret_cast<void*>(icon.get()), "rb");
  Serial.println("file is open");
  gdispImageError err = gdispImageOpenGFile(&image, imageData);

  if (err) {
    Serial.println("Error opening Image");
    Serial.println(err, HEX);
  }

  // Get the screen size
  width = gdispGetWidth();
  Serial.printf("width %d\n", width);
  height = gdispGetHeight();
  Serial.printf("height %d", height);
  Serial.println();

  Serial.printf("image width: %d\n", image.width);
  Serial.printf("image height: %d\n", image.height);

  // render image
  gdispClear(GFX_BLACK);
  gdispImageDraw(&image, 0, 0, image.width, image.height, 0, 0);
  gdispImageClose(&image);
  gfileClose(imageData);
  gdispFlush();

  gPowermode powerMode = gdispGetPowerMode();
  Serial.printf("powermode: %d", powerMode);
}

  // Initialize and clear the display
  

  // connectToWifi();
  // WeatherAPI api = WeatherAPI(cityId);
  // std::vector<WeatherData> weatherInfo;
  // api.fetchForecast(weatherInfo);

  // std::unique_ptr<uint8_t[]> icon;
  // try {
  //   icon = api.fetchWeatherIcon(weatherInfo[0].currentCondition.icon);
  // } catch (const std::logic_error &e) {
  //   Serial.println(F("Could not fetch weather icon"));
  //   Serial.println(e.what());
  //   return;
  // }

  // GFILE* imageData = gfileOpenMemory(reinterpret_cast<void*>(icon.get()), "rb");
  // Serial.println("file is open");
  // gdispImageError err = gdispImageOpenGFile(&image, imageData);

  // if (err) {
  //   Serial.println("Error opening Image");
  //   Serial.println(err, HEX);
  // }

  // // Get the screen size
  // coord_t width = gdispGetWidth();
  // Serial.printf("width %d\n", width);
  // coord_t height = gdispGetHeight();
  // Serial.printf("height %d", height);
  // Serial.println();

  // // render image
  // gdispClear(GFX_WHITE);
  // gdispImageDraw(&image, 0, 0, image.width, image.height, 0, 0);
  // gdispImageClose(&image);
  // gfileClose(imageData);
  // gdispFlush();

  // gPowermode powerMode = gdispGetPowerMode();
  // Serial.printf("powermode: %d", powerMode);
  // Serial.println();
  // unsigned displayCount = gdispGetDisplayCount();
  // Serial.printf("num displays %d", displayCount);
  // Serial.println();

  // GDisplay* display = gdispGetDisplay(0);

  // gPowermode powerMode = gdispGGetPowerMode(display);
  // Serial.printf("powermode: %d", powerMode);
  // Serial.println();

  // gdispSetPowerMode(gPowerDeepSleep);
  // powerMode = gdispGGetPowerMode(display);
  // Serial.printf("powermode: %d", powerMode);
  // Serial.println();

  // gdispSetPowerMode(gPowerOn);
  // powerMode = gdispGGetPowerMode(display);
  // Serial.printf("powermode: %d", powerMode);
  // Serial.println();

  // Code Here
  // Serial.println("clear screen");
  // gdispClear(GFX_WHITE);
  // // gdispFillArea(width / 2, height / 2, width / 2 - 10, height / 2 - 10, GFX_BLACK);
  // font_t font = gdispOpenFont("DejaVuSans16");
  // gdispDrawString(10, height / 3, "Dies ist ein Test!", font, GFX_BLACK);
  // gdispCloseFont(font);
  // for (i = 5, j = 0; i < width && j < height; i += 7, j += i / 20)
  //   gdispDrawPixel(i, j, GFX_WHITE);

  // Serial.println("flush screen");
  // gdispFlush();

  // Serial.println("sleep");
  // gdispSetPowerMode(gPowerDeepSleep);

//   powerMode = gdispGetPowerMode();
//   Serial.printf("powermode: %d", powerMode);
//   Serial.println();
// }

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

