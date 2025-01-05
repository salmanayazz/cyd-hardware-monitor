#include <TFT_eSPI.h>
#include <SPI.h>
#include <Arduino.h>
#include "HardwareData.h"

class Display {
public:
    Display() {
        tft.init();
        tft.fillScreen(TFT_BLACK);
        tft.setTextColor(TFT_WHITE);
        tft.setRotation(1);
        tft.setTextFont(1);
        tft.setTextSize(1);
        tft.setTextDatum(TL_DATUM);

        sprite.createSprite(tft.width(), tft.height());
        sprite.setTextColor(TFT_WHITE);
        sprite.setRotation(1);
        sprite.setTextFont(1);
        sprite.setTextSize(1);
        sprite.setTextDatum(TL_DATUM);
    }

    void print(HardwareData hardwareData) {
        drawCard("CPU", 0, 0, 60);
        drawBarGraph(hardwareData.cpuUsage, 0, 0, false, "%");
        drawBarGraph(hardwareData.cpuTemp, 0, 30, true, "C");
        sprite.pushSprite(0, 0);

        drawCard("GPU", 0, 0, 60);
        drawBarGraph(hardwareData.gpuUsage, 0, 0, false, "%");
        drawBarGraph(hardwareData.gpuTemp, 0, 30, true, "C");
        sprite.pushSprite(tft.width() / 2, 0);
        
        drawCard("MEMORY", 0, 0);
        drawBarGraph(hardwareData.memoryUsage, 0, 0, false, "%");
        sprite.pushSprite(0, 70);

        drawCard("FPS - " + hardwareData.fpsProcess, 0, 0);
        drawBarGraph(hardwareData.fps, 0, 0, false, "", true);
        sprite.pushSprite(tft.width() / 2, 70);
    }

private:
    TFT_eSPI tft = TFT_eSPI();
    TFT_eSprite sprite = TFT_eSprite(&tft);
    int barWidth = 5;
    int spacing = 1;
    int graphWidth = tft.width() / 2 - 10;
    int maxBars = graphWidth / (barWidth + spacing);
    int graphHeight = 30;

    void drawCard(String label, int x, int y, int height = 30) {
        int width = tft.width() / 2 - 10;
        sprite.createSprite(width + 5, height + 5);
        sprite.fillSprite(TFT_BLACK);

        sprite.fillRoundRect(5, 5, width, height, 5, 0x1082);
        sprite.drawString(label, 10, 0);
    }

    void drawBarGraph(const std::vector<int>& data, int x, int y, bool flipped, String unit, bool autoScale = false) {
        int startX = x + maxBars * (barWidth + spacing) + 10;
        int startY = y + 5;
        int barCount = min((int)data.size(), maxBars);
        int maxValue = 100;

        if (autoScale) { 
            for (int i = data.size() - 1; i >= data.size() - barCount && i >= 0; i--) {
                maxValue = max(maxValue, data[i]);
            }
        }

        for (int i = barCount - 1; i >= 0; i--) {
            int value = data[data.size() - 1 - i];
            int barHeight = map(value, 0, maxValue, 0, graphHeight);
            
            if (flipped) {
                sprite.fillRect(startX - i * (barWidth + spacing), startY, barWidth, barHeight, TFT_RED);
            } else {
                sprite.fillRect(startX - i * (barWidth + spacing), startY + (graphHeight - barHeight), barWidth, barHeight, TFT_BLUE);
            }
        }

        String value = data.size() > 0 ? String(data.back()) : "00";
        if (value.length() == 1) value = "0" + value;

        sprite.drawString(value + unit, x + maxBars * (barWidth + spacing) + 20, y + 15);
    }
};
