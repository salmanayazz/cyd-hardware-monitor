#include "HardwareData.h"
#include <Arduino.h>

class MetricPrefixData: public HardwareData {
public:
    MetricPrefixData(String title, String unit, int colour) : HardwareData(title, unit, colour) {
        
    }

    String getFormattedValue(int index) override {
    String value = "00";
    String metricPrefix = unitMap[0];

    if (index < data.size() && index > 0 && data[index] > 0) {
        int unitIndex = int(std::log(data[index]) / std::log(1024));

        float formattedValue = data[index] / std::pow(1024, unitIndex);

        if (formattedValue >= 100) {
            // change 3 digits value to higher unit
            unitIndex++;
            formattedValue /= 1024;
        } else {
            // keep at most one decimal place
            formattedValue = round(formattedValue * 10) / 10.0;
        }

        value = String(formattedValue, formattedValue < 10 ? 1 : 0); // keep one decimal if <10
        metricPrefix = unitMap[unitIndex];
    }

    if (value.length() == 1) {
        value = "0" + value;
    }

    return value + metricPrefix + unit;
}


private:
    String unitMap[5] = {"", "K", "M", "G", "T"};
    int maxValue = 0;
};