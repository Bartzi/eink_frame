#include "openweathermap_ui.h"

OpenWeatherMapUI::OpenWeatherMapUI(GDisplay* display) : WeatherUI(display) {}

void OpenWeatherMapUI::updateForecast(std::vector<WeatherData> weatherInfo, WeatherAPI* api) {
    Serial.println("Start Update");
    font_t timeFont = gdispOpenFont("DejaVuSans32");
    font_t infoFont = gdispOpenFont("DejaVuSans16");

    renderCenteredString("Das Wetter in " +  weatherInfo[0].city, 0, 34, 640, timeFont, GFX_BLACK);

    for (int i = 0; i < 4; ++i) {
        Serial.println("Fetch weather icon");
        // first fetch the icon
        std::unique_ptr<uint8_t[]> icon;
        try {
            icon = api->fetchWeatherIcon(weatherInfo[i].icon);
        } catch (const std::logic_error &e) {
            Serial.println(F("Could not fetch weather icon"));
            Serial.println(e.what());
            continue;
        }

        Serial.println("icon fetched");

        coord_t startX = (i + 1) * 16 + i * 140;
        coord_t startY = 100;
        coord_t boxWidth = 140;
        coord_t boxHeight = 250;
        Serial.println("build ui");
        // second build the ui
        gdispGDrawBox(display, startX, startY, boxWidth, boxHeight, GFX_BLACK);

        renderCenteredString(weatherInfo[i].date, startX, startY + 10, boxWidth, timeFont, GFX_BLACK);
        showImage(static_cast<void*>(icon.get()), startX, startY + 50, boxWidth);
        renderCenteredString(String(weatherInfo[i].temperature) + F(" C"), startX, startY + 130, boxWidth, infoFont, GFX_BLACK);
        renderCenteredString(String(weatherInfo[i].maxTemperature) + F(" C"), startX, startY + 160, boxWidth, infoFont, GFX_RED);
        renderCenteredString(String(weatherInfo[i].minTemperature) + F(" C"), startX, startY + 180, boxWidth, infoFont, GFX_BLACK);
        renderCenteredString(String(weatherInfo[i].humidity) + F(" %"), startX, startY + 200, boxWidth, infoFont, GFX_BLACK);
        renderCenteredString(String(weatherInfo[i].pressure) + F(" hPa"), startX, startY + 220, boxWidth, infoFont, GFX_BLACK);

        Serial.println("done building ui");
    }

    gdispGFlush(display);
    gdispCloseFont(timeFont);
    gdispCloseFont(infoFont);
}