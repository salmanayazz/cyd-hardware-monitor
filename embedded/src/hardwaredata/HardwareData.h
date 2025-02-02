#include <vector>

class HardwareData {
public:
    HardwareData(String title, String unit, int colour) {
        this->title = title;
        this->unit = unit;
        this->colour = colour;
    }

    virtual void push_back(int value) {
        if (data.size() > 100) {
            data.erase(data.begin());
        }
        data.push_back(value);
    }

    virtual int& operator[](std::size_t index) {
        return data[index];
    }

    virtual String getFormattedValue(int index) {
        String value = "00";

        if (index < data.size() && index > 0) {
            value = String(data[index]);
        }
        
        if (value.length() == 1) {
            value = "0" + value;
        }

        return value + unit;
    }
    
    int size() {
        return data.size();
    }

    virtual String getUnit() {
        return unit;
    }

    virtual String getTitle() {
        return title;
    }

    void setTitle(String title) {
        this->title = title;
    }

    virtual int getColour() {
        return colour;
    }

protected:
    std::vector<int> data = {};
    String title = "";
    String unit = "";
    int colour = 0;
};