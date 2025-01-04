import main
import hardware_stats
import asyncio
from bleak import BleakClient, BleakScanner
import json

BLE_SERVICE_UUID = "12345678-1234-1234-1234-1234567890ab"
BLE_CHARACTERISTIC_UUID = "87654321-4321-4321-4321-0987654321ba"

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

async def send_data(client):
    data = get_hardware_data()
    
    json_data = json.dumps(data)
    
    await client.write_gatt_char(BLE_CHARACTERISTIC_UUID, json_data.encode('utf-8'))
    print(f"Sent: {json_data}")

def get_hardware_data():
    gpu_temp, gpu_util = hardware_stats.get_gpu_stats()
    fps, process = hardware_stats.get_fps_stats()
    
    return {
        "cpuUtil": hardware_stats.get_cpu_util(),
        "cpuTemp": hardware_stats.get_cpu_temp(),
        "gpuUtil": gpu_util,
        "gpuTemp": gpu_temp,
        "memoryUtil": hardware_stats.get_memory_util(),
        "fps": fps,
        "fpsProcess": process
    }