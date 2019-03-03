#include "temperature.h"

TemperatureSensor::TemperatureSensor(void) {
    dht.setup(DHT_PIN, DHTesp::DHT22);
}

TempAndHumidity TemperatureSensor::getTempAndHumidity() {
    auto reading = dht.getTempAndHumidity();
    if (dht.getStatus() != 0) {
        Serial.println("DHT22 error: " + String(dht.getStatusString()));
        return TempAndHumidity();
    }
    return reading;
}