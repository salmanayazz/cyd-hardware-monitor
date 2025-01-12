#include <TFT_eSPI.h>
#include <SPI.h>
#include <Arduino.h>
#include "HardwareData.h"

class Display {
public:
    Display() {
        tft.init();
        tft.fillScreen(bgColor1);
        tft.setRotation(1);

        sprite.setTextColor(TFT_WHITE);
        sprite.setRotation(1);
        sprite.setTextFont(1);
        sprite.setTextSize(1);
        sprite.setTextDatum(TL_DATUM);
    }

    void draw(HardwareData* hardwareDataList) {
        drawDoubleBarGraph("CPU", hardwareDataList[0].cpuUsage, hardwareDataList[0].cpuTemp, "%", "C", 0, 0, false, false);
        drawDoubleBarGraph("GPU", hardwareDataList[0].gpuUsage, hardwareDataList[0].gpuTemp, "%", "C", tft.width() / 2, 0, false, false);
        drawBarGraph("MEMORY", hardwareDataList[0].memoryUsage, "%", 0, 70, false);
        drawBarGraph("FPS - " + hardwareDataList[0].fpsProcess, hardwareDataList[0].fps, "", tft.width() / 2, 70, true);
        drawDoubleBarGraph("CPU", hardwareDataList[1].cpuUsage, hardwareDataList[1].cpuTemp, "%", "C", 0, 130, false, false);
        drawBarGraph("MEMORY", hardwareDataList[1].memoryUsage, "%", 0, 200, false);
    }

private:
    TFT_eSPI tft = TFT_eSPI();
    TFT_eSprite sprite = TFT_eSprite(&tft);
    int itemHeight = 30;
    int itemWidth = max(tft.width(), tft.height()) / 2 - 10;
    int bgColor1 = TFT_BLACK;
    int bgColor2 = 0x1082;

    void drawCard(String label, int itemHeight, int width) {
        sprite.fillSprite(bgColor1);

        sprite.fillRoundRect(5, 5, width, itemHeight, 5, bgColor2);
        sprite.drawString(label, 10, 0);
    }

    void drawBars(std::vector<int>& data, bool flipped, String unit, bool autoScale = false) {
        int barWidth = 5;
        int spacing = 1;
        int maxBars = (itemWidth - 40) / (barWidth + spacing);
        int startX = maxBars * (barWidth + spacing) + 10;
        int startY = (flipped ? itemHeight : 0) + 5;
        int barCount = min((int)data.size(), maxBars);
        int maxValue = 100;

        if (autoScale) { 
            for (int i = data.size() - 1; i >= data.size() - barCount && i >= 0; i--) {
                maxValue = max(maxValue, data[i]);
            }
        }

        for (int i = barCount - 1; i >= 0; i--) {
            int value = data[data.size() - 1 - i];
            int barHeight = map(value, 0, maxValue, 0, itemHeight);
            
            if (flipped) {
                sprite.fillRect(startX - i * (barWidth + spacing), startY, barWidth, barHeight, TFT_RED);
            } else {
                sprite.fillRect(startX - i * (barWidth + spacing), startY + (itemHeight - barHeight), barWidth, barHeight, TFT_BLUE);
            }
        }

        String value = data.size() > 0 ? String(data.back()) : "00";
        if (value.length() == 1) value = "0" + value;

        sprite.drawString(value + unit, maxBars * (barWidth + spacing) + 20, startY + 10);
    }

    void drawBarGraph(String label, std::vector<int>& data, String unit, int x, int y, bool autoScale) {
        sprite.createSprite(itemWidth + 5, itemHeight + 5);

        drawCard(label, itemHeight, itemWidth);
        drawBars(data, false, unit, autoScale);

        sprite.pushSprite(x, y);
    }

    void drawDoubleBarGraph(String label, std::vector<int>& data1, std::vector<int>& data2, String unit1, String unit2, int x, int y, bool autoScale1, bool autoScale2) {
        sprite.createSprite(itemWidth + 5, itemHeight * 2 + 5);

        drawCard(label, itemHeight * 2, itemWidth);
        drawBars(data1, false, unit1, autoScale1);
        drawBars(data2, true, unit2, autoScale2);

        sprite.pushSprite(x, y);
    }
};
