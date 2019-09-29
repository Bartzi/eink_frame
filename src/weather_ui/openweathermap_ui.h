#pragma once

#include "weather_ui.h"
#include "weather/openweathermap_api.h"

class OpenWeatherMapUI : public WeatherUI {

    public:
        OpenWeatherMapUI(String cityId, GDisplay* display);
        void updateForecast();

    private:
        OpenWeatherMapAPI api;
};