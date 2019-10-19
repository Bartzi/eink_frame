#include "accuweather_ui.h"

AccuWeatherUI::AccuWeatherUI(GDisplay* display) : WeatherUI(display) {}

void AccuWeatherUI::updateForecast(std::vector<WeatherData> weatherInfo, WeatherAPI* api) {
    font_t largeFont = gdispOpenFont("DejaVuSans32");
    font_t smallFont = gdispOpenFont("DejaVuSans12");
    font_t mediumFont = gdispOpenFont("DejaVuSans20");
    font_t betweenFont = gdispOpenFont("DejaVuSans16");

    renderCenteredString("Das Wetter in " +  weatherInfo[0].city, 0, 34, 640, largeFont, GFX_BLACK);

    for (int i = 0; i < 4; ++i) {
        // first fetch the icon
        std::unique_ptr<uint8_t[]> icon;
        try {
            icon = api->fetchWeatherIcon(weatherInfo[i].icon);
        } catch (const std::logic_error &e) {
            Serial.println(F("Could not fetch weather icon"));
            Serial.println(e.what());
            continue;
        }

        coord_t startX = (i + 1) * 16 + i * 140;
        coord_t startY = 100;
        coord_t boxWidth = 140;
        coord_t boxHeight = 250;
        // second build the ui
        gdispGDrawBox(display, startX, startY, boxWidth, boxHeight, GFX_BLACK);

        renderCenteredString(weatherInfo[i].date, startX, startY + 10, boxWidth, largeFont, GFX_BLACK);
        showImage(static_cast<void*>(icon.get()), startX, startY + 50, boxWidth);
        renderCenteredString(String(weatherInfo[i].phrase), startX, startY + 120, boxWidth, smallFont, GFX_BLACK);
        renderCenteredString(String(weatherInfo[i].temperature) + F(" C"), startX, startY + 150, boxWidth, mediumFont, GFX_RED);
        renderCenteredString(String(weatherInfo[i].rainProbability) + F(" %"), startX, startY + 175, boxWidth, mediumFont, GFX_BLACK);
        renderCenteredString(String(weatherInfo[i].windSpeed) + F(" km/h (") + String(weatherInfo[i].windDirection) + F(")"), startX, startY + 200, boxWidth, betweenFont, GFX_BLACK);
        // renderCenteredString(String(F("aus ")) + String(weatherInfo[i].windDirection), startX, startY + 225, boxWidth, mediumFont, GFX_BLACK);
    }

    gdispGFlush(display);
    gdispCloseFont(largeFont);
    gdispCloseFont(smallFont);
    gdispCloseFont(mediumFont);
    gdispCloseFont(betweenFont);
}