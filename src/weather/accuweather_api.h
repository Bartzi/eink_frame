#pragma once

#include "weather_api.h"

class AccuWeatherAPI: public WeatherAPI {

    public:
        AccuWeatherAPI(String cityId);
        String fetchForecast(std::vector<WeatherData> &weatherData);
        // std::unique_ptr<uint8_t[]> fetchWeatherIcon(String iconId);

    protected:
        String getIconName(String iconId);

    private:
        String apiEndpoint = F("dataservice.accuweather.com");
        String cityName = F("Potsdam");
        int hourIndices[4] = {0, 3, 6, 9};
        std::map<String, String> iconMap {
            {"1", "sun"},
            {"2", "sun"},
            {"3", "partly-cloudy-day"},
            {"4", "partly-cloudy-day"},
            {"5", "dust"},
            {"6", "cloudy"},
            {"7", "clouds"},
            {"8", "clouds"},
            {"11", "fog-day"},
            {"12", "little-rain"},
            {"13", "little-rain"},
            {"14", "little-rain"},
            {"15", "storm"},
            {"16", "chance-of-storm"},
            {"17", "chance-of-storm"},
            {"18", "rain"},
            {"19", "clouds"},
            {"20", "cloudy"},
            {"21", "cloudy"},
            {"22", "snow"},
            {"23", "little-snow"},
            {"24", "icy"},
            {"25", "sleet"},
            {"26", "hail"},
            {"29", "sleet"},
            {"30", "thermometer-2"},
            {"31", "temperature-2"},
            {"32", "sun-5"},
            {"33", "moon-4"},
            {"34", "moon-4"},
            {"35", "partly-cloudy-night"},
            {"36", "partly-cloudy-night"},
            {"37", "fog-night"},
            {"38", "cloudy"},
            {"39", "little-rain"},
            {"40", "little-rain"},
            {"41", "storm"},
            {"42", "storm"},
            {"43", "clouds"},
            {"44", "snow"},
        };
};