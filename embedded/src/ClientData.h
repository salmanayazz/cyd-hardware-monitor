#include <vector>
#include "hardwaredata/HardwareData.h"

class ClientData {
public:
	HardwareData cpuUtil = HardwareData("CPU", "%", TFT_BLUE);
	HardwareData cpuTemp = HardwareData("CPU", "C", TFT_RED);
	HardwareData gpuUtil = HardwareData("GPU", "%", TFT_BLUE);
	HardwareData gpuTemp = HardwareData("GPU", "C", TFT_RED);
	HardwareData memoryUtil = HardwareData("MEMORY", "%", TFT_BLUE);
	HardwareData fps = HardwareData("FPS", "", TFT_BLUE);
};
  