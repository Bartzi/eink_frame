#pragma once

extern "C" {
  #include "gfx.h"
}

#include "weather/weather_api.h"

class WeatherUI {
    public:
        WeatherUI(String cityId, GDisplay* display);
        void updateForecast();

    private:
        void showImage(void* data, coord_t startX, coord_t startY, coord_t maxWidth);
        void renderCenteredString(String text, coord_t startX, coord_t startY, coord_t maxWidth, font_t font, gColor color);

        WeatherAPI api;
        GDisplay* display;
};