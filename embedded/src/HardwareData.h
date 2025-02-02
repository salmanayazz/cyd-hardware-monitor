#include <vector>

class HardwareData {
public:
	std::vector<int> cpuUsage = {};
	std::vector<int> cpuTemp = {};
	std::vector<int> gpuUsage = {};
	std::vector<int> gpuTemp = {};
	std::vector<int> memoryUsage = {};
	std::vector<int> fps = {};
	String fpsProcess = "";
};
  