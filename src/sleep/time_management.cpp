#include "time_management.h"

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

TimeManager::TimeManager(GDisplay* display) : display(display), timeClient(ntpUDP, "europe.pool.ntp.org") {
    timeClient.begin();
    bool success = timeClient.update();
    if (!success) {
        Serial.println("Could not get the current time, trying again for once!");
        timeClient.update();
    }
};

String TimeManager::getCurrentTime() {
    return timeClient.getFormattedTime();
}

uint TimeManager::getSleepTime(int wakeUpMinute) {
    int minutes = timeClient.getMinutes();
    int seconds = timeClient.getSeconds();
    int secondsToSleep = 60 - seconds;
    int minutesToSleep = 59 - (minutes + 1);
    return minutesToSleep * 60 + secondsToSleep;
}

void TimeManager::showTimeStamp() {
    font_t font = gdispOpenFont("DejaVuSans12");
    String text = String(F("Last Update: ")) + this->getCurrentTime() + " UTC";

    gdispGDrawStringBox(display, 0, gdispGGetHeight(display) - 15, 640, 15, text.c_str(), font, GFX_BLACK, justifyRight);

    gdispCloseFont(font);
}


void TimeManager::gotoSleep(bool errorSleep, int sleepMinutes) {
    print_wakeup_reason();

    uint sleepTimeInSeconds;
    if (errorSleep) {   
        sleepTimeInSeconds = sleepMinutes * 60;
    } else {
        sleepTimeInSeconds = this->getSleepTime(59);
    }

    esp_sleep_enable_timer_wakeup(sleepTimeInSeconds * uS_TO_S_FACTOR);
    Serial.println("Setup ESP32 to wake up in " + String(static_cast<int>(sleepTimeInSeconds / 60)) + " minutes");
    Serial.println("Going to sleep now");
    Serial.flush(); 
    esp_deep_sleep_start();
}