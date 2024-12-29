#include <TFT_eSPI.h>
#include <SPI.h>
#include <HardwareData.h>

class Display {
public:
    Display() {
        tft.init();
        tft.fillScreen(TFT_BLACK);
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
        tft.setRotation(1);
        tft.setTextFont(1);
        tft.setTextSize(1);
        tft.setTextDatum(TL_DATUM);
    }

    void print(HardwareData hardwareData) {
        drawBorders();

        drawBarGraph(hardwareData.cpuUsage, 0, 0, false, "%");
        drawBarGraph(hardwareData.cpuTemp, 0, 30, true, "C");
        drawBarGraph(hardwareData.gpuUsage, tft.width() / 2, 0, false, "%");
        drawBarGraph(hardwareData.gpuTemp, tft.width() / 2, 30, true, "C");
        drawBarGraph(hardwareData.ramUsage, 0, 70, false, "%");
    }

private:
    TFT_eSPI tft = TFT_eSPI();
    int barWidth = 5;
    int spacing = 1;
    int graphWidth = tft.width() / 2 - 10;
    int maxBars = graphWidth / (barWidth + spacing);
    int graphHeight = 30;
    int maxValue = 100;

    void drawBorders() {
        drawBorder("CPU", 0, 0, 60);
        drawBorder("GPU", tft.width() / 2, 0, 60);
        drawBorder("RAM", 0, 70);
    }

    void drawBorder(String label, int x, int y, int height = 30) {
        int width = tft.width() / 2 - 10;
        tft.fillRoundRect(x + 5, y + 5, width, height, 5, 0x1082);

        tft.drawString(label, x + 10, y);
    }

    void drawBarGraph(const std::vector<int>& data, int x, int y, bool flipped, String unit) {
        int startX = x + maxBars * (barWidth + spacing) + 10;
        int startY = y + 5;
        int barCount = min((int)data.size(), maxBars);

        for (int i = barCount - 1; i >= 0; i--) {
            int value = data[data.size() - 1 - i];
            int barHeight = map(value, 0, maxValue, 0, graphHeight);
            
            if (flipped) {
                tft.fillRect(startX - i * (barWidth + spacing), startY, barWidth, barHeight, TFT_RED);
            } else {
                tft.fillRect(startX - i * (barWidth + spacing), startY + (graphHeight - barHeight), barWidth, barHeight, TFT_BLUE);
            }
        }

        String value = "00";
        if (data.size() > 0) {
            value = String(data.back());
            if (value.length() == 1) {
                value = "0" + value;
            }
        }

        tft.drawString(value + unit, x + maxBars * (barWidth + spacing) + 20, y + 15);
    }
};
