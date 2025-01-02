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
    }

    void print(HardwareData hardwareData) {
        tft.fillScreen(TFT_BLACK);

        drawCard("CPU", 0, 0, 60);
        drawBarGraph(hardwareData.cpuUsage, 0, 0, false, "%");
        drawBarGraph(hardwareData.cpuTemp, 0, 30, true, "C");

        drawCard("GPU", tft.width() / 2, 0, 60);
        drawBarGraph(hardwareData.gpuUsage, tft.width() / 2, 0, false, "%");
        drawBarGraph(hardwareData.gpuTemp, tft.width() / 2, 30, true, "C");
        
        drawCard("MEMORY", 0, 70);
        drawBarGraph(hardwareData.memoryUsage, 0, 70, false, "%");

        drawCard("FPS - " + hardwareData.fpsProcess, tft.width() / 2, 70);
        drawBarGraph(hardwareData.fps, tft.width() / 2, 70, false, "", true);
    }

private:
    TFT_eSPI tft = TFT_eSPI();
    int barWidth = 5;
    int spacing = 1;
    int graphWidth = tft.width() / 2 - 10;
    int maxBars = graphWidth / (barWidth + spacing);
    int graphHeight = 30;

    void drawCard(String label, int x, int y, int height = 30) {
        int width = tft.width() / 2 - 10;
        
        tft.fillRoundRect(x + 5, y + 5, width, height, 5, 0x1082);
        tft.drawString(label, x + 10, y);
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
                tft.fillRect(startX - i * (barWidth + spacing), startY, barWidth, barHeight, TFT_RED);
            } else {
                tft.fillRect(startX - i * (barWidth + spacing), startY + (graphHeight - barHeight), barWidth, barHeight, TFT_BLUE);
            }
        }

        String value = data.size() > 0 ? String(data.back()) : "00";
        if (value.length() == 1) value = "0" + value;

        tft.drawString(value + unit, x + maxBars * (barWidth + spacing) + 20, y + 15);
    }
};
