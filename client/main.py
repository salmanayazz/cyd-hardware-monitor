import asyncio
from bleak import BleakClient, BleakScanner
import psutil
from pynvml import (
    nvmlInit, nvmlDeviceGetHandleByIndex, nvmlDeviceGetTemperature,
    nvmlDeviceGetUtilizationRates, nvmlShutdown
)
import clr
clr.AddReference(r'OpenHardwareMonitorLib')
from OpenHardwareMonitor.Hardware import Computer

BLE_SERVICE_UUID = "12345678-1234-1234-1234-1234567890ab"
BLE_CHARACTERISTIC_UUID = "87654321-4321-4321-4321-0987654321ba"
BLE_ADDRESS = "A0:B7:65:05:68:AE"

def get_cpu_temp():
    c = Computer()
    c.CPUEnabled = True
    c.Open()

    for hardware in c.Hardware:
        hardware.Update()
        for sensor in hardware.Sensors:
            if "temperature" in str(sensor.Identifier).lower():
                try:
                    temp = sensor.get_Value()
                    return temp
                except Exception as e:
                    print(f"Error getting temperature: {e}")

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

async def send_data(client):
    data = get_hardware_data()
    res = f"{data['cpuUtil']},{data['cpuTemp']},{data['gpuUtil']},{data['gpuTemp']},{data['memoryUtil']}"
    
    await client.write_gatt_char(BLE_CHARACTERISTIC_UUID, res.encode('utf-8'))
    print(f"Sent: {res.strip()}")

async def connect(address):
    client = BleakClient(
        address,
        use_cached_services=False
    )

    while True:
        print(f"Attempting to connect to {address}...")
        
        try:
            await asyncio.wait_for(client.connect(), timeout=10)
            print(f"Connected to {address}")
            
            while client.is_connected:
                await send_data(client)
                await asyncio.sleep(5)
        
        except asyncio.TimeoutError:
            print("Connection timed out. Reinitializing BleakClient...")
        
        except Exception as e:
            print(f"Failed to connect or send data: {e}")
        
        finally:
            print("Disconnected. Retrying in 5 seconds...")

        await client.disconnect()
        await asyncio.sleep(5)

async def main():
    await connect(BLE_ADDRESS)
        

if __name__ == "__main__":
    asyncio.run(main())
