#include <cstring>
#include <exception>

#include <HTTPClient.h>
#include "weather_api.h"
#include "api_credentials.h"

WeatherAPI::WeatherAPI(String cityId) : cityId(cityId), httpPort(80) {}

std::unique_ptr<uint8_t[]> WeatherAPI::fetchWeatherIcon(String iconId) {
    HTTPClient https;

    String iconName = getIconName(iconId);

    String url = this->iconEndpoint + "/icons/download/black/" + iconName + "-64.png";
    Serial.println(url);

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