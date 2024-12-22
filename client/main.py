import asyncio
from bleak import BleakClient
import psutil
from pynvml import (
    nvmlInit, nvmlDeviceGetHandleByIndex, nvmlDeviceGetTemperature,
    nvmlDeviceGetUtilizationRates, nvmlShutdown
)

BLE_SERVICE_UUID = "12345678-1234-1234-1234-1234567890ab"
BLE_CHARACTERISTIC_UUID = "87654321-4321-4321-4321-0987654321ba"
BLE_ADDRESS = "A0:B7:65:05:68:AE"


def get_cpu_temp():
    try:
        import wmi 
        w = wmi.WMI(namespace="root\OpenHardwareMonitor")
        sensors = w.Sensor()
        for sensor in sensors:
            if sensor.SensorType == 'Temperature' and 'CPU' in sensor.Name:
                return sensor.Value
    except Exception as e:
        print(f"Failed to fetch CPU temperature: {e}")
    return None



def get_cpu_util():
    return psutil.cpu_percent(interval=1)

def get_gpu_stats():
    gpu_temp, gpu_util = None, None
    try:
        nvmlInit()
        handle = nvmlDeviceGetHandleByIndex(0)
        gpu_temp = nvmlDeviceGetTemperature(handle, 0)
        gpu_util = nvmlDeviceGetUtilizationRates(handle).gpu
    except Exception as e:
        print(f"Failed to fetch GPU stats: {e}")
    
    return gpu_temp, gpu_util

def get_hardware_data():
    gpu_temp, gpu_util = get_gpu_stats()
    return {
        "cpuUtil": psutil.cpu_percent(),
        "cpuTemp": get_cpu_temp(),
        "gpuUtil": gpu_util,
        "gpuTemp": gpu_temp,
        "memoryUtil": psutil.virtual_memory().percent
    }

async def send_data(address):
    print(f"Connecting to device: {address}")
    async with BleakClient(address) as client:
        print(f"Connected")
        
        while True:
            
            data = get_hardware_data()
            res = f"{data['cpuUtil']},{data['cpuTemp']},{data['gpuUtil']},{data['gpuTemp']},{data['memoryUtil']}"
            
            await client.write_gatt_char(BLE_CHARACTERISTIC_UUID, res.encode('utf-8'))
            print(f"Sent: {res.strip()}")
            await asyncio.sleep(5)

async def main():
    await send_data(BLE_ADDRESS)

if __name__ == "__main__":
    asyncio.run(main())
