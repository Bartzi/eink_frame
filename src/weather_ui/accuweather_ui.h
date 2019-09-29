#pragma once

#include "weather_ui.h"
#include "weather/accuweather_api.h"

class AccuWeatherUI : public WeatherUI {

    public:
        AccuWeatherUI(String cityId, GDisplay* display);
        void updateForecast();

    private:
        AccuWeatherAPI api;
};