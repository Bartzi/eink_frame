#pragma once

#include "Arduino.h"
#include <NTPClient.h>
#include <WiFi.h>
#include <WiFiUdp.h>

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  60        /* Time ESP32 will go to sleep (in units) */
#define TIME_TO_SLEEP_BETWEEN_FAILURE 1


void print_wakeup_reason(){
    esp_sleep_wakeup_cause_t wakeup_reason;

    wakeup_reason = esp_sleep_get_wakeup_cause();

    switch(wakeup_reason)
    {
        case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
        case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
        case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
        case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
        case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
        default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
    }
}

uint getNTPBasedSleepTime(int wakeUpMinute) {
    WiFiUDP ntpUDP;
    NTPClient timeClient(ntpUDP, "europe.pool.ntp.org");
    timeClient.begin();
    
    bool success = timeClient.update();
    if (!success) {
        Serial.println("Could not get the current time, trying again for once!");
        timeClient.update();
    }

    Serial.println(timeClient.getFormattedTime());
    int minutes = timeClient.getMinutes();
    int seconds = timeClient.getSeconds();
    int secondsToSleep = 60 - seconds;
    int minutesToSleep = 59 - (minutes + 1);
    return minutesToSleep * 60 + secondsToSleep;
}

void gotoSleep(bool errorSleep) {
    print_wakeup_reason();

    uint sleepTimeInSeconds;
    if (errorSleep) {   
        sleepTimeInSeconds = 60;
    } else {
        sleepTimeInSeconds = getNTPBasedSleepTime(59);
    }

    esp_sleep_enable_timer_wakeup(sleepTimeInSeconds * uS_TO_S_FACTOR);
    Serial.println("Setup ESP32 to wake up in " + String(static_cast<int>(sleepTimeInSeconds / 60)) + " minutes");
    Serial.println("Going to sleep now");
    Serial.flush(); 
    esp_deep_sleep_start();
}