#pragma once

extern "C" {
  #include "gfx.h"
}

#include <Arduino.h>
#include <WiFiUdp.h>
#include <NTPClient.h>

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  60        /* Time ESP32 will go to sleep (in units) */
#define TIME_TO_SLEEP_BETWEEN_FAILURE 1


class TimeManager {
    public:
        TimeManager(GDisplay* display);
        String getCurrentTime();
        uint getSleepTime(int wakeUpMinute);
        void showTimeStamp();
        void gotoSleep(bool errorSleep, int sleepMinutes);

    private:
        GDisplay* display;
        WiFiUDP ntpUDP;
        NTPClient timeClient;;
};