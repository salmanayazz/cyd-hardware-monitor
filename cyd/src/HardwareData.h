class HardwareData {
public:
	HardwareData(int cpuTemp, int cpuUsage, int gpuTemp, int gpuUsage, int ramUsage) {
		this->cpuTemp = cpuTemp;
		this->cpuUsage = cpuUsage;
		this->gpuTemp = gpuTemp;
		this->gpuUsage = gpuUsage;
		this->ramUsage = ramUsage;
	}
	
    int cpuTemp = 0;
    int cpuUsage = 0;
    int gpuTemp = 0;
    int gpuUsage = 0;
    int ramUsage = 0;
};
  