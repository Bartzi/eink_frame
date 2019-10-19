#pragma once

#include <map>
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
    float rainProbability;
    float windSpeed;
    String windDirection;
    String icon;
    String date;
    String city;
    String phrase;
} WeatherData;


class WeatherAPI {
    public:
        WeatherAPI(String cityId);
        virtual void fetchForecast(std::vector<WeatherData> &weatherData) = 0;
        std::unique_ptr<uint8_t[]> fetchWeatherIcon(String iconId);

    protected:
        virtual String getIconName(String iconId) = 0;
        String cityId;
        String iconEndpoint = F("https://www.iconsdb.com");
        int httpPort;
        

        const char* iconCertificate =  \
        "-----BEGIN CERTIFICATE-----\n" \
        "MIIDozCCAougAwIBAgIQD/PmFjmqPRoSZfQfizTltjANBgkqhkiG9w0BAQsFADBa\n" \
        "MQswCQYDVQQGEwJJRTESMBAGA1UEChMJQmFsdGltb3JlMRMwEQYDVQQLEwpDeWJl\n" \
        "clRydXN0MSIwIAYDVQQDExlCYWx0aW1vcmUgQ3liZXJUcnVzdCBSb290MB4XDTE1\n" \
        "MTAxNDEyMDAwMFoXDTIwMTAwOTEyMDAwMFowbzELMAkGA1UEBhMCVVMxCzAJBgNV\n" \
        "BAgTAkNBMRYwFAYDVQQHEw1TYW4gRnJhbmNpc2NvMRkwFwYDVQQKExBDbG91ZEZs\n" \
        "YXJlLCBJbmMuMSAwHgYDVQQDExdDbG91ZEZsYXJlIEluYyBFQ0MgQ0EtMjBZMBMG\n" \
        "ByqGSM49AgEGCCqGSM49AwEHA0IABNFW9Jy25DGg9aRSz+Oaeob/8oayXsy1WcwR\n" \
        "x07dZP1VnGDjoEvZeFT/SFC6ouGhWHWPx2A3RBZNVZns7tQzeiOjggEZMIIBFTAS\n" \
        "BgNVHRMBAf8ECDAGAQH/AgEAMA4GA1UdDwEB/wQEAwIBhjA0BggrBgEFBQcBAQQo\n" \
        "MCYwJAYIKwYBBQUHMAGGGGh0dHA6Ly9vY3NwLmRpZ2ljZXJ0LmNvbTA6BgNVHR8E\n" \
        "MzAxMC+gLaArhilodHRwOi8vY3JsMy5kaWdpY2VydC5jb20vT21uaXJvb3QyMDI1\n" \
        "LmNybDA9BgNVHSAENjA0MDIGBFUdIAAwKjAoBggrBgEFBQcCARYcaHR0cHM6Ly93\n" \
        "d3cuZGlnaWNlcnQuY29tL0NQUzAdBgNVHQ4EFgQUPnQtH89FdQR+P8Cihz5MQ4NR\n" \
        "E8YwHwYDVR0jBBgwFoAU5Z1ZMIJHWMys+ghUNoZ7OrUETfAwDQYJKoZIhvcNAQEL\n" \
        "BQADggEBADhfp//8hfJzMuTVo4mZlmCvMsEDs2Xfvh4DyqXthbKPr0uMc48qjKkA\n" \
        "DgEkF/fsUoV2yOUcecrDF4dQtgQzNp4qnhgXljISr0PMVxje28fYiCWD5coGJTH9\n" \
        "vV1IO1EB3SwUx8FgUemVAdiyM1YOR2aNbM2v+YXZ6xxHR4g06PD6wqtPaU4JWdRX\n" \
        "xszByOPmGcFYOFLi4oOF3iI03D+m968kvOBvwKtoLVLHawVXLEIbLUiHAwyQq0hI\n" \
        "qSi+NIr7uu30YJkdFXgRqtltU39pKLy3ayB2f6BVA3F59WensKAKF1eyAKmtz/9n\n" \
        "jD4m5ackvMJvEOiJxnCl0h+A7Q0/JxM=\n" \
        "-----END CERTIFICATE-----\n";
};