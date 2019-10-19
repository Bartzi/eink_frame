#pragma once

#include "weather_ui.h"

class AccuWeatherUI : public WeatherUI {

    public:
        AccuWeatherUI(GDisplay* display);
        void updateForecast(std::vector<WeatherData> weatherInfo, WeatherAPI* api);
};