import asyncio
from bleak import BleakScanner

async def scan_ble_devices():
    print("Scanning for BLE devices...")
    devices = await BleakScanner.discover()
    for device in devices:
        print(f"Address: {device.address}, Name: {device.name or 'Unknown'}")

if __name__ == "__main__":
    asyncio.run(scan_ble_devices())