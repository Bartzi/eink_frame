#pragma once

#include <memory>
#include <vector>
#include <ArduinoJson.h>
#include <WiFi.h>

typedef struct {
    float temperature;
    float minTemperature;
    float maxTemperature;
    float pressure;
    float humidity;
    float windSpeed;
    float windDirection;
    String icon;
    String date;
    String city;
} WeatherData;


class WeatherAPI {
    public:
        WeatherAPI(String cityId);
        void fetchForecast(std::vector<WeatherData> &weatherData);
        std::unique_ptr<uint8_t[]> fetchWeatherIcon(String iconId);

    private:
        String apiEndpoint = F("api.openweathermap.org");
        String cityId;
        String iconEndpoint = F("http://openweathermap.org");
        int httpPort;
};