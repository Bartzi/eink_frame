#pragma once
#include "Arduino.h"

extern "C" {
  #include "gfx.h"
}

class WeatherUI {
    public:
        WeatherUI(GDisplay* display);
        virtual void updateForecast() = 0;
        void showConnectionError(String ssid, String timeout);

    protected:
        void showImage(void* data, coord_t startX, coord_t startY, coord_t maxWidth);
        void renderCenteredString(String text, coord_t startX, coord_t startY, coord_t maxWidth, font_t font, gColor color);
        GDisplay* display;
};