#include <TFT_eSPI.h>
#include <SPI.h>
#include <Arduino.h>
#include "ClientData.h"

class Display {
public:
    Display() {
        tft.init();
        tft.fillScreen(bgColor1);
        tft.setRotation(1);
        tft.setTextColor(TFT_WHITE);
        tft.setTextFont(1);
        tft.setTextSize(1);
        tft.setTextDatum(TL_DATUM);

        sprite.setTextColor(TFT_WHITE);
        sprite.setRotation(1);
        sprite.setTextFont(1);
        sprite.setTextSize(1);
        sprite.setTextDatum(TL_DATUM);
    }

    void draw(ClientData* clientDataList) {
        tft.drawString("PRIMARY", 10, 0);
        drawDoubleBarGraph("CPU", clientDataList[0].cpuUtil, clientDataList[0].cpuTemp, 0, 10, itemWidth);
        drawDoubleBarGraph("GPU", clientDataList[0].gpuUtil, clientDataList[0].gpuTemp, tft.width() / 2, 10, itemWidth);

        drawBarGraph("MEMORY", clientDataList[0].memoryUtil, 0, itemHeight * 2 + 10, itemWidth);
        drawBarGraph("FPS", clientDataList[0].fps, tft.width() / 2, itemHeight * 2 + 10, itemWidth);

        tft.drawString("SECONDARY", 10, itemHeight * 3 + 20);
        drawDoubleBarGraph("CPU", clientDataList[1].cpuUtil, clientDataList[1].cpuTemp, 0, itemHeight * 3 + 30, itemWidth);
        drawBarGraph("MEMORY", clientDataList[1].memoryUtil, 0, itemHeight * 5 + 30, itemWidth);
    }

private:
    TFT_eSPI tft = TFT_eSPI();
    TFT_eSprite sprite = TFT_eSprite(&tft);
    int itemHeight = 35;
    int itemWidth = max(tft.width(), tft.height()) / 2;
    int bgColor1 = TFT_BLACK;
    int bgColor2 = 0x1082;

    void drawCard(int width, int height) {
        sprite.fillSprite(bgColor1);
        sprite.fillRoundRect(5, 5, width - 5, height - 5, 5, bgColor2);
    }

    void drawBars(HardwareData& hardwareData, bool flipped, int width) {
        int barWidth = 3;
        int spacing = 1;
        int maxBars = (width - 45) / (barWidth + spacing);
        int startX = maxBars * (barWidth + spacing) + 10;
        int startY = itemHeight;
        int barCount = min((int)hardwareData.size(), maxBars);
        int maxValue = 100;

        for (int i = hardwareData.size() - 1; i >= hardwareData.size() - barCount && i >= 0; i--) {
            maxValue = max(maxValue, hardwareData[i]);
        }

        for (int i = barCount - 1; i >= 0; i--) {
            int value = hardwareData[hardwareData.size() - 1 - i];
            int barHeight = map(value, 0, maxValue, 1, itemHeight - 5);
            int currentX = startX - i * (barWidth + spacing);
            int currentY = startY - (flipped ? 0 : barHeight);
        
            sprite.fillRect(currentX, currentY, barWidth, barHeight, hardwareData.getColour());
        }

        String value = hardwareData.getFormattedValue(hardwareData.size() - 1);

        sprite.drawString(value, maxBars * (barWidth + spacing) + 20, (flipped ? startY : 0) + 15);
    }

    void drawBarGraph(String label, HardwareData& hardwareData, int x, int y, int width) {
        sprite.createSprite(width, itemHeight);

        drawCard(width, itemHeight);
        drawBars(hardwareData, false, width);
        sprite.drawString(hardwareData.getTitle(), 10, 0);

        sprite.pushSprite(x, y);
        sprite.deleteSprite();
    }

    void drawDoubleBarGraph(String label, HardwareData& data1, HardwareData& data2, int x, int y, int width) {
        sprite.createSprite(width, itemHeight * 2);

        drawCard(width, itemHeight * 2 - 5);
        drawBars(data1, false, width);
        drawBars(data2, true, width);
        sprite.drawString(data1.getTitle(), 10, 0);

        sprite.pushSprite(x, y);
        sprite.deleteSprite();
    }
};
