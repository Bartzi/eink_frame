#include <cstring>
#include <exception>

#include <HTTPClient.h>
#include "api_credentials.h"

#include "openweathermap_api.h"

OpenWeatherMapAPI::OpenWeatherMapAPI(String cityId) : WeatherAPI(cityId) {
    this->iconMap = {
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
}

String OpenWeatherMapAPI::fetchForecast(std::vector<WeatherData> &weatherData) {
    WiFiClient client;
    String message;

    if (!client.connect(this->apiEndpoint.c_str(), this->httpPort)) {
        message = F("Can not Connect to Weather API!");
        Serial.println(message);
        return message;
    }

    String url = "/data/2.5/forecast?id=" + this->cityId + "&APPID=" + OPENWEATHERMAP_API_KEY + "&units=metric&cnt=4";
    Serial.println(this->apiEndpoint);
    Serial.println(url);

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

    char jsonArray[result.length() + 1];
    result.toCharArray(jsonArray, sizeof(jsonArray));
    jsonArray[result.length() + 1] = '\0';
    Serial.println(result.length());

    StaticJsonDocument<2500> jsonData;
    DeserializationError err = deserializeJson(jsonData, jsonArray);
    Serial.println(jsonData.memoryUsage());

    if (err) {
        message = String(F("deserializeJson() failed with code ")) + String(err.c_str());
        Serial.println(message);
        return message;
    }
    
    Serial.println("parsing weather info into data structure");
    for (JsonObject timeStep : jsonData["list"].as<JsonArray>()) {
        WeatherData data;
        JsonObject mainData = timeStep["main"];
        data.temperature = mainData["temp"];    
        data.minTemperature = mainData["temp_min"];
        data.maxTemperature = mainData["temp_max"];
        data.pressure = mainData["pressure"];
        data.humidity = mainData["humidity"];
        data.windSpeed = timeStep["wind"]["speed"];
        data.windDirection = String(timeStep["wind"]["deg"].as<char*>());

        JsonObject condition = timeStep["weather"][0];
        data.icon = String(condition["icon"].as<char*>());

        data.date = String(timeStep["dt_txt"].as<char*>()).substring(10, 16);
        data.city = String(jsonData["city"]["name"].as<char*>());
        Serial.println(data.city);

        weatherData.push_back(data);
    }

    client.stop();
    return "";
}

String OpenWeatherMapAPI::getIconName(String iconId) {
    return iconMap[iconId];
}
