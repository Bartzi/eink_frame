#include <cstring>
#include <exception>

#include <HTTPClient.h>

#include "accuweather_api.h"
#include "api_credentials.h"

AccuWeatherAPI::AccuWeatherAPI(String cityId) : WeatherAPI(cityId) {}

String AccuWeatherAPI::fetchForecast(std::vector<WeatherData> &weatherData) {
    WiFiClient client;
    String message;

    Serial.println("connect to api endpoint");

    if (!client.connect(this->apiEndpoint.c_str(), this->httpPort)) {
        message = F("Can not Connect to Weather API!");
        Serial.println(message);
        return message;
    }

    String url = "/forecasts/v1/hourly/12hour/" + this->cityId + "?apikey=" + ACCUWEATHER_API_KEY + "&language=de-de&details=true&metric=true";

    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                "Host: " + this->apiEndpoint + "\r\n" +
                "Connection: close\r\n\r\n");

    unsigned long timeout = millis();
    while (client.available() == 0) {
        if (millis() - timeout > 5000) {
            message = F("Timeout while fetching data from weatherforecast Server");
            Serial.println(message);
            client.stop();
            return message;
        }
    }

    String result = "";
    while(client.available()) {
        result = client.readStringUntil('\r');
    }

    DynamicJsonDocument jsonData(35000);
    DeserializationError err = deserializeJson(jsonData, result);

    if (err) {
        message = String(F("deserializeJson() failed with code ")) + String(err.c_str());
        Serial.println(message);
        return message;
    }

    JsonObject root = jsonData.as<JsonObject>();

    if (!root["Code"].isNull()) {
        // we got an error from the server!
        message = root["Message"].as<String>();
        Serial.println(message);
        return message;
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
    return "";
}

String AccuWeatherAPI::getIconName(String iconId) {
    return iconMap[iconId];
}
