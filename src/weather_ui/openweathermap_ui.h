#pragma once

#include "weather_ui.h"

class OpenWeatherMapUI : public WeatherUI {

    public:
        OpenWeatherMapUI(GDisplay* display);
        void updateForecast(std::vector<WeatherData> weatherInfo, WeatherAPI* api);

};