#pragma once

#include "weather_api.h"

class OpenWeatherMapAPI : public WeatherAPI {
    public:
        OpenWeatherMapAPI(String cityId);
        void fetchForecast(std::vector<WeatherData> &weatherData);
        std::unique_ptr<uint8_t[]> fetchWeatherIcon(String iconId);

    private:
        String apiEndpoint = F("api.openweathermap.org");

    private:
        std::map<String, String> iconMap {
            {"01d", "sun"},
            {"02d", "partly-cloudy-day"},
            {"03d", "cloudy"},
            {"04d", "clouds"},
            {"09d", "little-rain"},
            {"10d", "rain"},
            {"11d", "storm"},
            {"13d", "snow"},
            {"50d", "fog-day"},
            {"01n", "moon-4"},
            {"02n", "partly-cloudy-night"},
            {"03n", "cloudy"},
            {"04n", "clouds"},
            {"09n", "little-rain"},
            {"10n", "rain"},
            {"11n", "storm"},
            {"13n", "snow"},
            {"50n", "fog-night"}
        };
};