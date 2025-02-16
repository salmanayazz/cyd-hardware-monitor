#include <vector>
#include "hardwaredata/HardwareData.h"
#include "hardwaredata/MetricPrefixData.h"

class ClientData {
public:
	HardwareData* cpuUtil = new HardwareData("CPU", "%", TFT_BLUE);
	HardwareData* cpuTemp = new HardwareData("CPU", "C", TFT_RED);
	HardwareData* gpuUtil = new HardwareData("GPU", "%", TFT_BLUE);
	HardwareData* gpuTemp = new HardwareData("GPU", "C", TFT_RED);
	HardwareData* memoryUtil = new HardwareData("MEMORY", "%", TFT_BLUE);
	HardwareData* fps = new HardwareData("FPS", "", TFT_BLUE);
	String fpsProcess = "";
	HardwareData* driveSpace = new HardwareData("DRIVE SPACE", "%", TFT_BLUE);
	HardwareData* driveRead = new MetricPrefixData("DRIVE R/W", "B", TFT_BLUE);
	HardwareData* driveWrite = new MetricPrefixData("DRIVE R/W", "B", TFT_RED);
	HardwareData* networkUpload = new MetricPrefixData("NETWORK U/D", "B", TFT_BLUE);
	HardwareData* networkDownload = new MetricPrefixData("NETWORK U/D", "B", TFT_RED);
};
  