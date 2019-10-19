#include <cstring>
#include <exception>

#include <HTTPClient.h>

#include "accuweather_api.h"
#include "api_credentials.h"

AccuWeatherAPI::AccuWeatherAPI(String cityId) : WeatherAPI(cityId) {}

void AccuWeatherAPI::fetchForecast(std::vector<WeatherData> &weatherData) {
    WiFiClient client;

    Serial.println("connect to api endpoint");

    if (!client.connect(this->apiEndpoint.c_str(), this->httpPort)) {
        Serial.println(F("Can not Connect to Weather API!"));
        return;
    }

    String url = "/forecasts/v1/hourly/12hour/" + this->cityId + "?apikey=" + ACCUWEATHER_API_KEY + "&language=de-de&details=true&metric=true";

    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                "Host: " + this->apiEndpoint + "\r\n" +
                "Connection: close\r\n\r\n");

    unsigned long timeout = millis();
    while (client.available() == 0) {
        if (millis() - timeout > 5000) {
            Serial.println(F("Timeout while fetching data from weatherforecast Server"));
            client.stop();
            return;
        }
    }

    String result = "";
    while(client.available()) {
        result = client.readStringUntil('\r');
    }

    DynamicJsonDocument jsonData(25000);
    DeserializationError err = deserializeJson(jsonData, result); 

    if (err) {
        Serial.print(F("deserializeJson() failed with code "));
        Serial.println(err.c_str());
        return;
    }

    for (int index : hourIndices) {
        WeatherData data;
        JsonObject hourData = jsonData.as<JsonArray>()[index];
        JsonObject temperatureObject = hourData["Temperature"];
        data.temperature = temperatureObject["Value"];
        data.icon = String(hourData["WeatherIcon"].as<signed int>());
        data.phrase = String(hourData["IconPhrase"].as<char*>());
        data.humidity = hourData["RelativeHumidity"];
        JsonObject windData = hourData["Wind"];
        data.windSpeed = windData["Speed"]["Value"];
        data.windDirection = String(windData["Direction"]["Localized"].as<char*>());
        data.rainProbability = hourData["PrecipitationProbability"];

        data.date = String(hourData["DateTime"].as<char*>()).substring(11, 16);
        data.city = cityName;

        weatherData.push_back(data);
    }

    client.stop();
}

String AccuWeatherAPI::getIconName(String iconId) {
    return iconMap[iconId];
}
