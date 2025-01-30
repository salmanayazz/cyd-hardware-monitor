import main
import hardware_stats
import asyncio
from bleak import BleakClient, BleakScanner
import json
import asyncio

BLE_SERVICE_UUID = "12345678-1234-1234-1234-1234567890ab"
BLE_CHARACTERISTIC_UUID = "87654321-4321-4321-4321-0987654321ba"

async def connect(address, is_primary_device):
    while True:
        print(f"Attempting to connect to {address}...")

        client = BleakClient(
            address,
            use_cached_services=False
        )

        try:
            await asyncio.wait_for(client.connect(), timeout=10)
            print(f"Connected to {address}")
            
            while client.is_connected:
                await send_data(client, is_primary_device)
                await asyncio.sleep(5)

        except asyncio.TimeoutError:
            print("Connection timed out. Reinitializing BleakClient...")

        except Exception as e:
            print(f"Failed to connect or send data: {e}")

        finally:
            if client.is_connected:
                await client.disconnect()
            print("Disconnected. Retrying in 5 seconds...")

        await asyncio.sleep(5)


async def send_data(client, is_primary_device):
    try: 
        data = await get_hardware_data(is_primary_device)
        
        json_data = json.dumps(data)
        
        await client.write_gatt_char(BLE_CHARACTERISTIC_UUID, json_data.encode('utf-8'))
        print(f"Sent: {json_data}")
    except Exception as e:
        print(f"Failed to send data: {e}")


async def get_hardware_data(is_primary_device):
    if is_primary_device:
        gpu_temp, gpu_util = hardware_stats.get_gpu_stats()
        
        fps, process = hardware_stats.get_fps_stats()
        
        cpu_util, cpu_temp, gpu_stats, memory_util, fps_stats = await asyncio.gather(
            asyncio.to_thread(hardware_stats.get_cpu_util),
            asyncio.to_thread(hardware_stats.get_cpu_temp),
            asyncio.to_thread(hardware_stats.get_gpu_stats),
            asyncio.to_thread(hardware_stats.get_memory_util),
            asyncio.to_thread(hardware_stats.get_fps_stats)   
        )

        gpu_temp, gpu_util = gpu_stats
        fps, process = fps_stats
        
        return {
            "isPrimary": True,
            "cpuUtil": cpu_util,
            "cpuTemp": cpu_temp,
            "gpuUtil": gpu_util,
            "gpuTemp": gpu_temp,
            "memoryUtil": memory_util,
            "fps": fps,
            "fpsProcess": process
        }
    else:
        cpu_util, cpu_temp, memory_util = await asyncio.gather(
            asyncio.to_thread(hardware_stats.get_cpu_util),
            asyncio.to_thread(hardware_stats.get_cpu_temp),
            asyncio.to_thread(hardware_stats.get_memory_util)
        )

        return {
            "isPrimary": False,
            "cpuUtil": cpu_util,
            "cpuTemp": cpu_temp,
            "memoryUtil": memory_util,
        }