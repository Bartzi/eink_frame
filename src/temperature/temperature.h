#pragma once

#include <DHTesp.h>

#define DHT_PIN 23

class TemperatureSensor {
    public:
        TemperatureSensor();
        TempAndHumidity getTempAndHumidity();

    private:
        DHTesp dht;
};