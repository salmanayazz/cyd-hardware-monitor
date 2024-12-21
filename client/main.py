import asyncio
from bleak import BleakClient

BLE_SERVICE_UUID = "12345678-1234-1234-1234-1234567890ab"
BLE_CHARACTERISTIC_UUID = "87654321-4321-4321-4321-0987654321ba"

def getSystemStats():
    
    return {
        "cpuTemp": 1,
        "cpuUtil": 2,
        "gpuTemp": 3,
        "gpuUtil": 4,
    }

async def sendData(address):
    async with BleakClient(address) as client:
        print(f"Connected to BLE device: {address}")
        
        while True:
            stats = getSystemStats()
            data = f"{stats['cpuTemp']},{stats['cpuUtil']},{stats['gpuTemp']},{stats['gpuUtil']}"
            
            await client.write_gatt_char(BLE_CHARACTERISTIC_UUID, data.encode('utf-8'))
            print(f"Sent: {data.strip()}")
            await asyncio.sleep(5)

async def main():
    bleAddress = "..."
    await sendData(bleAddress)

if __name__ == "__main__":
    asyncio.run(main())
