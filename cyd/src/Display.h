#include <TFT_eSPI.h>
#include <SPI.h>

class Display {
public:
    Display() {
        tft.init();
        tft.fillScreen(TFT_BLACK);
        tft.setTextColor(TFT_WHITE, TFT_BLACK);

        tft.setRotation(1);
        tft.setTextFont(1);
        tft.setTextSize(2);
        tft.setTextDatum(TL_DATUM);
    }

    void print(String text) {
        tft.fillScreen(TFT_BLACK);
        tft.setCursor(0, 0);
        tft.println(text);
    }

private:
    TFT_eSPI tft = TFT_eSPI();
};