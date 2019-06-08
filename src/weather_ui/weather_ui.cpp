#include <vector>

#include "weather_ui.h"

WeatherUI::WeatherUI(String cityId, GDisplay* display): api(cityId), display(display) {};

void WeatherUI::updateForecast() {
    font_t timeFont = gdispOpenFont("DejaVuSans32");
    font_t infoFont = gdispOpenFont("DejaVuSans16");

    std::vector<WeatherData> weatherInfo;
    api.fetchForecast(weatherInfo);

    renderCenteredString("Das Wetter in " +  weatherInfo[0].city, 0, 34, 640, timeFont, GFX_BLACK);

    for (int i = 0; i < 4; ++i) {
        // first fetch the icon
        std::unique_ptr<uint8_t[]> icon;
        try {
            icon = api.fetchWeatherIcon(weatherInfo[i].icon);
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

        renderCenteredString(weatherInfo[i].date, startX, startY + 10, boxWidth, timeFont, GFX_BLACK);
        showImage(static_cast<void*>(icon.get()), startX, startY + 55, boxWidth);
        renderCenteredString(String(weatherInfo[i].temperature) + F(" C"), startX, startY + 115, boxWidth, infoFont, GFX_BLACK);
        renderCenteredString(String(weatherInfo[i].maxTemperature) + F(" C"), startX, startY + 145, boxWidth, infoFont, GFX_RED);
        renderCenteredString(String(weatherInfo[i].minTemperature) + F(" C"), startX, startY + 165, boxWidth, infoFont, GFX_BLACK);
        renderCenteredString(String(weatherInfo[i].humidity) + F(" %"), startX, startY + 185, boxWidth, infoFont, GFX_BLACK);
        renderCenteredString(String(weatherInfo[i].pressure) + F(" hPa"), startX, startY + 205, boxWidth, infoFont, GFX_BLACK);
    }

    gdispGFlush(display);
    gdispCloseFont(timeFont);
    gdispCloseFont(infoFont);
}

void WeatherUI::showImage(void* data, coord_t startX, coord_t startY, coord_t maxWidth) {
    gdispImage image;
    GFILE* imageData = gfileOpenMemory(data, "rb");
    gdispImageError err = gdispImageOpenGFile(&image, imageData);

    if (err) {
        Serial.println("Error opening Image");
        Serial.println(err, HEX);
        gfileClose(imageData);
        return;
    }

    // render image
    coord_t imageStartX = startX + (maxWidth - image.width) / 2;
    coord_t imageStartY = startY;
    
    gdispGFillArea(display, imageStartX, imageStartY, image.width, image.height, GFX_RED);
    gdispImageDraw(&image, imageStartX, imageStartY, image.width, image.height, 0, 0);
    gdispImageClose(&image);
    gfileClose(imageData);
}

void WeatherUI::renderCenteredString(String text, coord_t startX, coord_t startY, coord_t maxWidth, font_t font, gColor color) {
    coord_t stringWidth = gdispGetStringWidth(text.c_str(), font);
    gdispGDrawString(display, startX + (maxWidth - stringWidth) / 2, startY, text.c_str(), font, color);
}