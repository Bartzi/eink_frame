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

    String url = "/forecasts/v1/hourly/12hour/" + this->cityId + "?apikey=" + ACCUWEATHER_API_KEY + "&language=de-de&details=false&metric=true";

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

    DynamicJsonDocument jsonData(5000);
    DeserializationError err = deserializeJson(jsonData, jsonArray); 

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
        // data.humidity = hourData["RelativeHumidity"];
        // JsonObject windData = hourData["Wind"];
        // data.windSpeed = windData["Speed"]["Value"];
        // data.windDirection = windData["Direction"]["Localized"];
        data.rainProbability = hourData["PrecipitationProbability"];

        data.date = String(hourData["DateTime"].as<char*>()).substring(11, 16);
        data.city = cityName;

        weatherData.push_back(data);
    }

    client.stop();
}

std::unique_ptr<uint8_t[]> AccuWeatherAPI::fetchWeatherIcon(String iconId) {
    HTTPClient https;

    String iconName = this->iconMap[iconId];

    String url = this->iconEndpoint + "/icons/download/black/" + iconName + "-64.png";

    if (!https.begin(url, iconCertificate)) {
        Serial.println(F("[HTTP] Can not establish connection to Server."));
        throw std::logic_error("Can not establish HTTP connection!");
    }

    int httpCode = https.GET();
    Serial.print(F("[HTTP] GET... code: "));
    Serial.println(httpCode);

    if (httpCode != HTTP_CODE_OK) {
        Serial.print(F("[HTTP] GET... failed, error: "));
        Serial.println(https.errorToString(httpCode));
        https.end();
        throw std::logic_error("HTTP Code not OK");
    }

    int total = https.getSize();
    int remaining = total;

    uint8_t buffer[128] = {0};
    std::unique_ptr<uint8_t[]> destination{new uint8_t[total]};
    int position = 0;

    WiFiClient* stream = https.getStreamPtr();

    while(https.connected() && (remaining > 0 || remaining == -1)) {
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
    https.end();
    return std::move(destination);
}