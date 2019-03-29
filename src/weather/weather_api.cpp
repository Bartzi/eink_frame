#include <cstring>
#include <exception>

#include <HTTPClient.h>
#include "weather_api.h"
#include "api_credentials.h"

WeatherAPI::WeatherAPI(String cityId) : cityId(cityId), httpPort(80) {}

void WeatherAPI::fetchForecast(std::vector<WeatherData> &weatherData) {
    WiFiClient client;

    if (!client.connect(this->apiEndpoint.c_str(), this->httpPort)) {
        Serial.println(F("Can not Connect to Weather API!"));
        return;
    }

    String url = "/data/2.5/forecast?id=" + this->cityId + "&APPID=" + OPENWEATHERMAP_API_KEY + "&units=metric&cnt=4";

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

    char jsonArray[result.length() + 1];
    result.toCharArray(jsonArray, sizeof(jsonArray));
    jsonArray[result.length() + 1] = '\0';

    StaticJsonDocument<2500> jsonData;
    DeserializationError err = deserializeJson(jsonData, jsonArray);
    Serial.println(jsonData.memoryUsage());

    if (err) {
        Serial.print(F("deserializeJson() failed with code "));
        Serial.println(err.c_str());
        return;
    }
    
    for (JsonObject timeStep : jsonData["list"].as<JsonArray>()) {
        WeatherData data;
        JsonObject mainData = timeStep["main"];
        data.temperature = mainData["temp"];
        data.minTemperature = mainData["temp_min"];
        data.maxTemperature = mainData["temp_max"];
        data.pressure = mainData["pressure"];
        data.humidity = mainData["humidity"];
        data.windSpeed = timeStep["wind"]["speed"];
        data.windDirection = timeStep["wind"]["deg"];

        JsonObject condition = timeStep["weather"][0];
        data.icon = String(condition["icon"].as<char*>());

        weatherData.push_back(data);
    }

    client.stop();
}

std::unique_ptr<uint8_t[]> WeatherAPI::fetchWeatherIcon(String iconId) {
    HTTPClient http;

    String url = this->iconEndpoint + "/img/w/" + iconId + ".png";
    http.begin(url);

    int httpCode = http.GET();
    Serial.print(F("[HTTP] GET... code: "));
    Serial.println(httpCode);

    if (httpCode != HTTP_CODE_OK) {
        Serial.print(F("[HTTP] GET... failed, error: "));
        Serial.println(http.errorToString(httpCode));
        http.end();
        throw std::logic_error("HTTP Code not OK");
    }

    int total = http.getSize();
    int remaining = total;

    uint8_t buffer[128] = {0};
    std::unique_ptr<uint8_t[]> destination{new uint8_t[total]};
    int position = 0;

    WiFiClient* stream = http.getStreamPtr();

    while(http.connected() && (remaining > 0 || remaining == -1)) {
        size_t size = stream->available();

        if (size) {
            int count = stream->readBytes(buffer, (size > sizeof(buffer)) ? sizeof(buffer) : size);
            if (position + count <= total) {
                std::memcpy(destination.get() + position, &buffer, count);
            } else {
                Serial.print(F("[HTTP] Got too much data for destination!"));
                Serial.print(String("got ") + String(position + count - total) + " bytes too much\n");
                throw std::out_of_range("Got too much data!");
            }
            position += count;
            if (remaining > 0) {
                remaining -= count;
            }
        }
        delay(1);
    }
    
    Serial.println(F("[HTTP] Finished Download"));
    http.end();
    return std::move(destination);
}