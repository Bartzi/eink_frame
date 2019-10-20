#include <vector>

#include "weather_ui.h"

WeatherUI::WeatherUI(GDisplay* display): display(display) {}

void WeatherUI::showErrorMessage(String message, String extraInfo, String timeout) {
    font_t font = gdispOpenFont("DejaVuSans20");
    String text = String(F("Could not connect to WiFi!"));
    renderCenteredString(message, 0, 150, 640, font, GFX_RED);

    if (extraInfo.length() > 0) {
        text = String(F("(")) + extraInfo + F(")");
        renderCenteredString(text, 0, 190, 640, font, GFX_BLACK);
    }

    text = String(F("Trying again in ")) + timeout + F(" minutes");
    renderCenteredString(text, 0, 230, 640, font, GFX_BLACK);

    gdispGFlush(display);
    gdispCloseFont(font);
}

void WeatherUI::showTimeStamp(String formattedTime) {
    font_t font = gdispOpenFont("DejavuSans12");
    String text = String(F("Letztes Update: ")) + formattedTime + " UTC";

    gdispGDrawStringBox(display, 0, gdispGGetHeight(display) - 15, 640, 15, text.c_str(), font, GFX_BLACK, justifyRight);

    gdispGFlush(display);
    gdispCloseFont(font);
}

void WeatherUI::showImage(void* data, coord_t startX, coord_t startY, coord_t maxWidth) {
    gdispImage image;
    GFILE* imageData = gfileOpenMemory(data, "rb");
    gdispImageError err = gdispImageOpenGFile(&image, imageData);

    if (err) {
        Serial.println("Error opening Image");
        Serial.println(err, HEX);
        gfileClose(imageData);
        return;
    }

    // render image
    coord_t imageStartX = startX + (maxWidth - image.width) / 2;
    coord_t imageStartY = startY;
    
    gdispImageDraw(&image, imageStartX, imageStartY, image.width, image.height, 0, 0);
    gdispImageClose(&image);
    gfileClose(imageData);
}

void WeatherUI::renderCenteredString(String text, coord_t startX, coord_t startY, coord_t maxWidth, font_t font, gColor color) {
    coord_t stringWidth = gdispGetStringWidth(text.c_str(), font);
    gdispGDrawString(display, startX + (maxWidth - stringWidth) / 2, startY, text.c_str(), font, color);
}